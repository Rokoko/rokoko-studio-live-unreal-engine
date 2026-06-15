// Copyright Rokoko. All Rights Reserved.

#include "Devices/LiveLinkRokokoDevice.h"

#include "Dom/JsonObject.h"
#include "Engine/Engine.h"
#include "Http/RokokoHttpClient.h"
#include "Misc/App.h"

#if ROKOKO_WITH_LIVELINKHUB
#include "Features/IModularFeatures.h"
#include "LiveLinkHub/ILiveLinkRecordingSession.h"
#include "NamingTokensEngineSubsystem.h"
#endif

DEFINE_LOG_CATEGORY(LogLiveLinkRokokoDevice);

#define LOCTEXT_NAMESPACE "LiveLinkRokokoDevice"

EDeviceHealth ULiveLinkRokokoDevice::GetDeviceHealth() const
{
    if (bAuthFailed)
    {
        return EDeviceHealth::Error;
    }

    switch (ConnectionStatus)
    {
    case ELiveLinkDeviceConnectionStatus::Connected:
        return bIsRecording ? EDeviceHealth::Good : EDeviceHealth::Nominal;
    case ELiveLinkDeviceConnectionStatus::Connecting:
        return EDeviceHealth::Info;
    case ELiveLinkDeviceConnectionStatus::Disconnecting:
        return EDeviceHealth::Warning;
    case ELiveLinkDeviceConnectionStatus::Disconnected:
    default:
        return LastErrorText.IsEmpty() ? EDeviceHealth::Warning : EDeviceHealth::Error;
    }
}

FText ULiveLinkRokokoDevice::GetHealthText() const
{
    if (bAuthFailed)
    {
        return LOCTEXT("AuthFailed", "Authentication failed. Verify API key.");
    }

    if (!LastErrorText.IsEmpty() && ConnectionStatus == ELiveLinkDeviceConnectionStatus::Disconnected)
    {
        return FText::FromString(LastErrorText);
    }

    switch (ConnectionStatus)
    {
    case ELiveLinkDeviceConnectionStatus::Connected:
        return bIsRecording ? LOCTEXT("ConnectedRecording", "Connected - Recording") : LOCTEXT("ConnectedReady", "Connected - Ready");
    case ELiveLinkDeviceConnectionStatus::Connecting:
        return LOCTEXT("Connecting", "Connecting...");
    case ELiveLinkDeviceConnectionStatus::Disconnecting:
        return LOCTEXT("Disconnecting", "Disconnecting...");
    case ELiveLinkDeviceConnectionStatus::Disconnected:
    default:
        return LOCTEXT("Disconnected", "Disconnected");
    }
}

void ULiveLinkRokokoDevice::OnDeviceAdded()
{
    Super::OnDeviceAdded();

#if ROKOKO_WITH_LIVELINKHUB
    if (IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkRecordingSession::GetModularFeatureName()))
    {
        ILiveLinkRecordingSession& Session = ILiveLinkRecordingSession::Get();
        Session.OnSlateNameChanged().AddUObject(this, &ULiveLinkRokokoDevice::HandleSlateNameChanged);
        Session.OnTakeNumberChanged().AddUObject(this, &ULiveLinkRokokoDevice::HandleTakeNumberChanged);
    }
#endif

    RecomputeFilename();
    ConnectInternal();
}

void ULiveLinkRokokoDevice::OnDeviceRemoved()
{
#if ROKOKO_WITH_LIVELINKHUB
    if (IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkRecordingSession::GetModularFeatureName()))
    {
        ILiveLinkRecordingSession& Session = ILiveLinkRecordingSession::Get();
        Session.OnSlateNameChanged().RemoveAll(this);
        Session.OnTakeNumberChanged().RemoveAll(this);
    }
#endif

    DisconnectInternal(true);

    Super::OnDeviceRemoved();
}

void ULiveLinkRokokoDevice::OnSettingChanged(const FPropertyChangedEvent& InPropertyChangedEvent)
{
    Super::OnSettingChanged(InPropertyChangedEvent);

    const FName ChangedProperty = InPropertyChangedEvent.GetPropertyName();

    static const FName HostName = GET_MEMBER_NAME_CHECKED(ULiveLinkRokokoDeviceSettings, Host);
    static const FName PortName = GET_MEMBER_NAME_CHECKED(ULiveLinkRokokoDeviceSettings, Port);
    static const FName ApiKeyName = GET_MEMBER_NAME_CHECKED(ULiveLinkRokokoDeviceSettings, ApiKey);
    static const FName FilenameFormatName = GET_MEMBER_NAME_CHECKED(ULiveLinkRokokoDeviceSettings, FilenameFormat);

    if (ChangedProperty == HostName || ChangedProperty == PortName || ChangedProperty == ApiKeyName)
    {
        bReconnectAfterSettingsChange = true;
        DisconnectInternal(true);
    }
    else if (ChangedProperty == FilenameFormatName)
    {
        RecomputeFilename();
    }
}

ELiveLinkDeviceConnectionStatus ULiveLinkRokokoDevice::GetConnectionStatus_Implementation() const
{
    return ConnectionStatus;
}

FString ULiveLinkRokokoDevice::GetHardwareId_Implementation() const
{
    const ULiveLinkRokokoDeviceSettings* DeviceSettings = GetSettings();
    return FString::Printf(TEXT("%s:%d"), *DeviceSettings->Host, DeviceSettings->Port);
}

bool ULiveLinkRokokoDevice::Connect_Implementation()
{
    if (ConnectionStatus == ELiveLinkDeviceConnectionStatus::Connected
        || ConnectionStatus == ELiveLinkDeviceConnectionStatus::Connecting)
    {
        return false;
    }

    bIntentionalDisconnect = false;
    ConnectInternal();
    return true;
}

bool ULiveLinkRokokoDevice::Disconnect_Implementation()
{
    if (ConnectionStatus == ELiveLinkDeviceConnectionStatus::Disconnected)
    {
        return false;
    }

    DisconnectInternal(true);
    return true;
}

void ULiveLinkRokokoDevice::SetConnectionStatus(ELiveLinkDeviceConnectionStatus InStatus)
{
    ConnectionStatus = InStatus;
    ILiveLinkDeviceCapability_Connection::SetConnectionStatus(InStatus);
}

bool ULiveLinkRokokoDevice::StartRecording_Implementation()
{
    if (ConnectionStatus != ELiveLinkDeviceConnectionStatus::Connected || bIsRecording)
    {
        return false;
    }

#if ROKOKO_WITH_LIVELINKHUB
    if (IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkRecordingSession::GetModularFeatureName()))
    {
        ILiveLinkRecordingSession& Session = ILiveLinkRecordingSession::Get();
        LastSlate = Session.GetSlateName();
        LastTake = Session.GetTakeNumber();
    }
#endif

    RecomputeFilename();

    TSharedRef<FJsonObject> Payload = MakeShared<FJsonObject>();
    if (!EvaluatedFilename.IsEmpty())
    {
        Payload->SetStringField(TEXT("filename"), EvaluatedFilename);
    }

    const ULiveLinkRokokoDeviceSettings* DeviceSettings = GetSettings();
    if (DeviceSettings->bOverrideFrameRate)
    {
        Payload->SetNumberField(TEXT("frame_rate"), DeviceSettings->FrameRate);
    }

    if (DeviceSettings->bSendSmpteTime)
    {
        Payload->SetStringField(TEXT("time"), FApp::GetTimecode().ToString());
    }

    if (!HttpClient.IsValid())
    {
        return false;
    }

    const bool bRequestQueued = HttpClient->PostCommand(
        MakeHttpConfig(),
        TEXT("recording/start"),
        Payload,
        [WeakThis = TWeakObjectPtr<ULiveLinkRokokoDevice>(this)](FRokokoHttpResponse&& Response)
        {
            ULiveLinkRokokoDevice* This = WeakThis.Get();
            if (!This)
            {
                return;
            }

            if (Response.bSucceeded)
            {
                This->bIsRecording = true;
                This->LastErrorText.Empty();
                return;
            }

            This->HandleRequestFailure(Response, TEXT("recording/start"), true);
        });

    if (!bRequestQueued)
    {
        LastErrorText = TEXT("Failed to queue recording/start request.");
    }

    return bRequestQueued;
}

bool ULiveLinkRokokoDevice::StopRecording_Implementation()
{
    if (ConnectionStatus != ELiveLinkDeviceConnectionStatus::Connected)
    {
        return false;
    }

    TSharedRef<FJsonObject> Payload = MakeShared<FJsonObject>();

    const ULiveLinkRokokoDeviceSettings* DeviceSettings = GetSettings();
    Payload->SetBoolField(TEXT("back_to_live"), DeviceSettings->bBackToLiveOnStop);

    if (DeviceSettings->bSendSmpteTime)
    {
        Payload->SetStringField(TEXT("time"), FApp::GetTimecode().ToString());
    }

    if (!HttpClient.IsValid())
    {
        return false;
    }

    const bool bRequestQueued = HttpClient->PostCommand(
        MakeHttpConfig(),
        TEXT("recording/stop"),
        Payload,
        [WeakThis = TWeakObjectPtr<ULiveLinkRokokoDevice>(this)](FRokokoHttpResponse&& Response)
        {
            ULiveLinkRokokoDevice* This = WeakThis.Get();
            if (!This)
            {
                return;
            }

            if (Response.bSucceeded)
            {
                This->bIsRecording = false;
                This->LastErrorText.Empty();
                return;
            }

            This->HandleRequestFailure(Response, TEXT("recording/stop"), true);
        });

    if (!bRequestQueued)
    {
        LastErrorText = TEXT("Failed to queue recording/stop request.");
    }

    return bRequestQueued;
}

bool ULiveLinkRokokoDevice::IsRecording_Implementation() const
{
    return bIsRecording;
}

void ULiveLinkRokokoDevice::ConnectInternal()
{
    if (ConnectionStatus != ELiveLinkDeviceConnectionStatus::Disconnected)
    {
        return;
    }

    if (!HttpClient.IsValid())
    {
        HttpClient = MakeShared<FRokokoHttpClient>();
    }

    bAuthFailed = false;
    SetConnectionStatus(ELiveLinkDeviceConnectionStatus::Connecting);

    ProbeConnection();
}

void ULiveLinkRokokoDevice::DisconnectInternal(bool bInIntentionalDisconnect)
{
    bIntentionalDisconnect = bInIntentionalDisconnect;

    if (ConnectionStatus == ELiveLinkDeviceConnectionStatus::Disconnected)
    {
        if (bReconnectAfterSettingsChange)
        {
            bReconnectAfterSettingsChange = false;
            bIntentionalDisconnect = false;
            ConnectInternal();
        }
        return;
    }

    SetConnectionStatus(ELiveLinkDeviceConnectionStatus::Disconnecting);

    if (HttpClient.IsValid())
    {
        HttpClient->CancelAllRequests();
    }

    bIsRecording = false;
    SetConnectionStatus(ELiveLinkDeviceConnectionStatus::Disconnected);

    ClearReconnectTicker();

    bIntentionalDisconnect = false;

    if (bReconnectAfterSettingsChange)
    {
        bReconnectAfterSettingsChange = false;
        ConnectInternal();
    }
}

void ULiveLinkRokokoDevice::ProbeConnection()
{
    TSharedRef<FJsonObject> Payload = MakeShared<FJsonObject>();

    if (!HttpClient.IsValid())
    {
        SetConnectionStatus(ELiveLinkDeviceConnectionStatus::Disconnected);
        LastErrorText = TEXT("HTTP client unavailable.");
        return;
    }

    const bool bRequestQueued = HttpClient->PostCommand(
        MakeHttpConfig(),
        TEXT("info"),
        Payload,
        [WeakThis = TWeakObjectPtr<ULiveLinkRokokoDevice>(this)](FRokokoHttpResponse&& Response)
        {
            ULiveLinkRokokoDevice* This = WeakThis.Get();
            if (!This)
            {
                return;
            }

            if (Response.bSucceeded)
            {
                This->LastErrorText.Empty();
                This->ClearReconnectTicker();
                This->SetConnectionStatus(ELiveLinkDeviceConnectionStatus::Connected);
                return;
            }

            This->SetConnectionStatus(ELiveLinkDeviceConnectionStatus::Disconnected);
            This->HandleRequestFailure(Response, TEXT("info"), false);

            if (!This->bIntentionalDisconnect && !This->bAuthFailed)
            {
                This->ScheduleReconnect();
            }
        });

    if (!bRequestQueued)
    {
        SetConnectionStatus(ELiveLinkDeviceConnectionStatus::Disconnected);
        LastErrorText = TEXT("Failed to queue info probe request.");
        ScheduleReconnect();
    }
}

void ULiveLinkRokokoDevice::RecomputeFilename()
{
    const ULiveLinkRokokoDeviceSettings* DeviceSettings = GetSettings();
    EvaluatedFilename = DeviceSettings->FilenameFormat;

#if ROKOKO_WITH_LIVELINKHUB
    if (!GEngine)
    {
        return;
    }

    UNamingTokensEngineSubsystem* TokenSubsystem = GEngine->GetEngineSubsystem<UNamingTokensEngineSubsystem>();
    if (!TokenSubsystem)
    {
        return;
    }

    if (IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkRecordingSession::GetModularFeatureName()))
    {
        ILiveLinkRecordingSession& Session = ILiveLinkRecordingSession::Get();
        LastSlate = Session.GetSlateName();
        LastTake = Session.GetTakeNumber();
    }

    FNamingTokenFilterArgs FilterArgs;
    FilterArgs.AdditionalNamespacesToInclude.Add(TEXT("llh"));

    const FNamingTokenResultData Result = TokenSubsystem->EvaluateTokenText(FText::FromString(DeviceSettings->FilenameFormat), FilterArgs);
    EvaluatedFilename = Result.EvaluatedText.ToString();
#endif

    UE_LOGFMT(LogLiveLinkRokokoDevice, Log, "Rokoko Studio recording filename set to '{Filename}'.",
        EvaluatedFilename);
}

void ULiveLinkRokokoDevice::HandleSlateNameChanged(FStringView InSlateName)
{
    LastSlate = FString(InSlateName);
    RecomputeFilename();
}

void ULiveLinkRokokoDevice::HandleTakeNumberChanged(int32 InTakeNumber)
{
    LastTake = InTakeNumber;
    RecomputeFilename();
}

void ULiveLinkRokokoDevice::HandleRequestFailure(const FRokokoHttpResponse& InResponse, const FString& InAction, bool bAffectsConnection)
{
    bAuthFailed = InResponse.HttpStatusCode == 401 || InResponse.HttpStatusCode == 403;

    if (bAuthFailed)
    {
        LastErrorText = TEXT("Authentication failed. Verify API key.");
    }
    else if (!InResponse.ErrorText.IsEmpty())
    {
        LastErrorText = InResponse.ErrorText;
    }
    else
    {
        LastErrorText = FString::Printf(TEXT("Command '%s' failed."), *InAction);
    }

    UE_LOG(LogLiveLinkRokokoDevice, Warning, TEXT("%s"), *LastErrorText);

    if (bAffectsConnection)
    {
        bIsRecording = false;

        if (ConnectionStatus == ELiveLinkDeviceConnectionStatus::Connected)
        {
            SetConnectionStatus(ELiveLinkDeviceConnectionStatus::Disconnected);

            if (!bAuthFailed)
            {
                ScheduleReconnect();
            }
        }
    }
}

void ULiveLinkRokokoDevice::ScheduleReconnect()
{
    if (ReconnectTickerHandle.IsValid())
    {
        return;
    }

    ReconnectTickerHandle = FTSTicker::GetCoreTicker().AddTicker(
        FTickerDelegate::CreateWeakLambda(this, [this](float) -> bool
        {
            ClearReconnectTicker();

            if (ConnectionStatus == ELiveLinkDeviceConnectionStatus::Disconnected)
            {
                ConnectInternal();
            }

            return false;
        }),
        ReconnectDelaySeconds);
}

void ULiveLinkRokokoDevice::ClearReconnectTicker()
{
    if (ReconnectTickerHandle.IsValid())
    {
        FTSTicker::GetCoreTicker().RemoveTicker(ReconnectTickerHandle);
        ReconnectTickerHandle.Reset();
    }
}

FRokokoHttpRequestConfig ULiveLinkRokokoDevice::MakeHttpConfig() const
{
    FRokokoHttpRequestConfig Config;
    const ULiveLinkRokokoDeviceSettings* DeviceSettings = GetSettings();

    Config.Host = DeviceSettings->Host;
    Config.Port = DeviceSettings->Port;
    Config.ApiKey = DeviceSettings->ApiKey;
    Config.ApiVersion = TEXT("v1");
    Config.TimeoutSeconds = 3.0f;

    return Config;
}

#undef LOCTEXT_NAMESPACE
