// Copyright Rokoko. All Rights Reserved.

#include "Customization/LiveLinkRokokoDeviceSettingsCustomization.h"
#include "Devices/LiveLinkRokokoDevice.h"
#include "Engine/Engine.h"
#include "LiveLinkDeviceSubsystem.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "Misc/CoreDelegates.h"

class FLiveLinkRokokoDeviceModule : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        FPropertyEditorModule& PropertyModule =
            FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

        PropertyModule.RegisterCustomClassLayout(
            ULiveLinkRokokoDeviceSettings::StaticClass()->GetFName(),
            FOnGetDetailCustomizationInstance::CreateStatic(
                &FLiveLinkRokokoDeviceSettingsCustomization::MakeInstance));

        PropertyModule.NotifyCustomizationModuleChanged();

        PostEngineInitHandle = FCoreDelegates::GetOnPostEngineInit().AddRaw(this, &FLiveLinkRokokoDeviceModule::TryAutoRegisterDevice);

        // Handle the case where the module loads after engine startup has already completed.
        if (GEngine)
        {
            TryAutoRegisterDevice();
        }
    }

    virtual void ShutdownModule() override
    {
        if (PostEngineInitHandle.IsValid())
        {
            FCoreDelegates::GetOnPostEngineInit().Remove(PostEngineInitHandle);
            PostEngineInitHandle.Reset();
        }

        if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
        {
            FPropertyEditorModule& PropertyModule =
                FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

            PropertyModule.UnregisterCustomClassLayout(
                ULiveLinkRokokoDeviceSettings::StaticClass()->GetFName());
        }
    }

private:
    void TryAutoRegisterDevice()
    {
        if (!GEngine)
        {
            return;
        }

        ULiveLinkDeviceSubsystem* DeviceSubsystem = GEngine->GetEngineSubsystem<ULiveLinkDeviceSubsystem>();
        if (!DeviceSubsystem)
        {
            UE_LOG(LogLiveLinkRokokoDevice, Warning, TEXT("LiveLinkDeviceSubsystem unavailable; skipping auto-registration."));
            return;
        }

        TArray<ULiveLinkDevice*> ExistingDevices;
        DeviceSubsystem->GetDevicesByClass(ULiveLinkRokokoDevice::StaticClass(), ExistingDevices);
        if (!ExistingDevices.IsEmpty())
        {
            UE_LOG(LogLiveLinkRokokoDevice, Verbose, TEXT("Rokoko device already present; skipping auto-registration."));
            return;
        }

        ULiveLinkRokokoDeviceSettings* SettingsTemplate = NewObject<ULiveLinkRokokoDeviceSettings>(GetTransientPackage());
        SettingsTemplate->DisplayName = TEXT("Rokoko Studio Device");

        ULiveLinkDeviceSubsystem::FCreateResult CreateResult =
            DeviceSubsystem->CreateDeviceOfClass(ULiveLinkRokokoDevice::StaticClass(), SettingsTemplate);

        if (!CreateResult.HasValue())
        {
            UE_LOG(LogLiveLinkRokokoDevice, Warning, TEXT("Failed to auto-register default Rokoko Live Link device."));
            return;
        }

        UE_LOG(LogLiveLinkRokokoDevice, Log, TEXT("Auto-registered default Rokoko Live Link device."));
    }

private:
    FDelegateHandle PostEngineInitHandle;
};

IMPLEMENT_MODULE(FLiveLinkRokokoDeviceModule, LiveLinkRokokoDevice);
