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
    FDelegateHandle PostEngineInitHandle;
};

IMPLEMENT_MODULE(FLiveLinkRokokoDeviceModule, LiveLinkRokokoDevice);
