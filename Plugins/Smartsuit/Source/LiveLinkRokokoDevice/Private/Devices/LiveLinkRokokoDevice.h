// Copyright Rokoko. All Rights Reserved.

#pragma once

#include "Containers/Ticker.h"
#include "LiveLinkDevice.h"
#include "LiveLinkDeviceCapability_Connection.h"
#include "LiveLinkDeviceCapability_Recording.h"
#include "LiveLinkRokokoDevice.generated.h"

class FRokokoHttpClient;
struct FRokokoHttpRequestConfig;
struct FRokokoHttpResponse;

DECLARE_LOG_CATEGORY_EXTERN(LogLiveLinkRokokoDevice, Log, All);

UCLASS()
class ULiveLinkRokokoDeviceSettings : public ULiveLinkDeviceSettings
{
    GENERATED_BODY()

public:
    ULiveLinkRokokoDeviceSettings()
    {
#if ROKOKO_WITH_LIVELINKHUB
        DisplayName = TEXT("Rokoko Studio Device");
#endif
    }

    UPROPERTY(EditAnywhere, Category = "Rokoko Device")
    FString Host = TEXT("127.0.0.1");

    UPROPERTY(EditAnywhere, Category = "Rokoko Device")
    int32 Port = 14053;

    UPROPERTY(EditAnywhere, Category = "Rokoko Device", meta = (PasswordField = true))
    FString ApiKey = TEXT("1234");

    UPROPERTY(EditAnywhere, Category = "Rokoko Device", meta = (NamingTokens))
    FString FilenameFormat = TEXT("{session}/{slate}_tk{take}");

    UPROPERTY(EditAnywhere, Category = "Rokoko Device")
    bool bOverrideFrameRate = false;

    UPROPERTY(EditAnywhere, Category = "Rokoko Device", meta = (EditCondition = "bOverrideFrameRate", ClampMin = "1", UIMin = "1"))
    int32 FrameRate = 24;

    UPROPERTY(EditAnywhere, Category = "Rokoko Device")
    bool bBackToLiveOnStop = true;

    UPROPERTY(EditAnywhere, Category = "Rokoko Device")
    bool bSendSmpteTime = false;
};

UCLASS(BlueprintType, meta = (DisplayName = "Rokoko Studio Device"))
class ULiveLinkRokokoDevice : public ULiveLinkDevice
    , public ILiveLinkDeviceCapability_Connection
    , public ILiveLinkDeviceCapability_Recording
{
    GENERATED_BODY()

public:
    ULiveLinkRokokoDeviceSettings* GetSettings() { return GetDeviceSettings<ULiveLinkRokokoDeviceSettings>(); }
    const ULiveLinkRokokoDeviceSettings* GetSettings() const { return GetDeviceSettings<ULiveLinkRokokoDeviceSettings>(); }

    //~ Begin ULiveLinkDevice interface
    virtual TSubclassOf<ULiveLinkDeviceSettings> GetSettingsClass() const override { return ULiveLinkRokokoDeviceSettings::StaticClass(); }
    virtual EDeviceHealth GetDeviceHealth() const override;
    virtual FText GetHealthText() const override;
    virtual void OnDeviceAdded() override;
    virtual void OnDeviceRemoved() override;
    virtual void OnSettingChanged(const FPropertyChangedEvent& InPropertyChangedEvent) override;
    //~ End ULiveLinkDevice interface

    //~ Begin ILiveLinkDeviceCapability_Connection interface
    virtual ELiveLinkDeviceConnectionStatus GetConnectionStatus_Implementation() const override;
    virtual FString GetHardwareId_Implementation() const override;
    virtual bool SetHardwareId_Implementation(const FString& InHardwareID) override { return false; }
    virtual bool Connect_Implementation() override;
    virtual bool Disconnect_Implementation() override;

protected:
    virtual void SetConnectionStatus(ELiveLinkDeviceConnectionStatus InStatus) override;
    //~ End ILiveLinkDeviceCapability_Connection interface

public:
    //~ Begin ILiveLinkDeviceCapability_Recording interface
    virtual bool StartRecording_Implementation() override;
    virtual bool StopRecording_Implementation() override;
    virtual bool IsRecording_Implementation() const override;
    //~ End ILiveLinkDeviceCapability_Recording interface

private:
    void ConnectInternal();
    void DisconnectInternal(bool bInIntentionalDisconnect);

    void ProbeConnection();
    void RecomputeFilename();

    void HandleSlateNameChanged(FStringView InSlateName);
    void HandleTakeNumberChanged(int32 InTakeNumber);

    void HandleRequestFailure(const FRokokoHttpResponse& InResponse, const FString& InAction, bool bAffectsConnection);
    void ScheduleReconnect();
    void ClearReconnectTicker();

    FRokokoHttpRequestConfig MakeHttpConfig() const;

private:
    ELiveLinkDeviceConnectionStatus ConnectionStatus = ELiveLinkDeviceConnectionStatus::Disconnected;

    TSharedPtr<FRokokoHttpClient> HttpClient;

    bool bIsRecording = false;
    bool bAuthFailed = false;
    bool bIntentionalDisconnect = false;
    bool bReconnectAfterSettingsChange = false;

    FString LastErrorText;
    FString EvaluatedFilename;
    FString LastSlate;
    int32 LastTake = 1;

    FTSTicker::FDelegateHandle ReconnectTickerHandle;

    static constexpr float ReconnectDelaySeconds = 3.0f;
};
