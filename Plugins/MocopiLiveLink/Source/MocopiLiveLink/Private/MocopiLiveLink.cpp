// 2023 Copyright Sony Corporation, Inc. All Rights Reserved

#include "MocopiLiveLink.h"

#include "SonyMotionFormat.h"

#include "Interfaces/IPluginManager.h"

#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY(LogMocopiLiveLink);

void FMocopiLiveLinkModule::StartupModule()
{

  // Set Localization based on current editor language
  FCultureRef culture = FInternationalization::Get().GetCurrentLanguage();
  FString ISOLanguageName = culture.Get().GetTwoLetterISOLanguageName();
  SetPluginLocalization(ISOLanguageName);

  // Adding a callback to be notified of any culture changes, so we can switch our language
  mCultureChangedHandle = FInternationalization::Get().OnCultureChanged().AddRaw(this, &FMocopiLiveLinkModule::OnCultureChanged);

  // Link to DLL Upon Plugin Load
  const FString pluginBaseDir = IPluginManager::Get().FindPlugin(TEXT("MocopiLiveLink"))->GetBaseDir();
  SonyMotionFormat::Initialize(std::string(TCHAR_TO_UTF8(*pluginBaseDir)));

}

void FMocopiLiveLinkModule::ShutdownModule()
{
  // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
  // we call this function before unloading the module.

  FInternationalization::Get().OnCultureChanged().Remove(mCultureChangedHandle);

  SonyMotionFormat::Shutdown();
}

void FMocopiLiveLinkModule::OnCultureChanged()
{
  FCultureRef lang = FInternationalization::Get().GetCurrentLanguage();
  FString ISOLanguageName = lang.Get().GetTwoLetterISOLanguageName();
  SetPluginLocalization(ISOLanguageName);
}

void FMocopiLiveLinkModule::SetPluginLocalization(FString ISOLanguageCode)
{
  // Get the localization path for the plugin
  const FString pluginBaseDir = IPluginManager::Get().FindPlugin(TEXT("MocopiLiveLink"))->GetBaseDir();
  FString localizationPath = FPaths::Combine(pluginBaseDir, "Resources", "Localization");

  FString locResFilePath = FPaths::Combine(localizationPath, ISOLanguageCode, TEXT("MocopiLiveLink.locres"));
  if (FPaths::FileExists(locResFilePath))
  {
    // Set plugin localization based on passed in ISO code
    FTextLocalizationManager::Get().UpdateFromLocalizationResource(locResFilePath);
    return;
  }

  // Default to English 
  locResFilePath = FPaths::Combine(localizationPath, "en", TEXT("MocopiLiveLink.locres"));
  if (FPaths::FileExists(locResFilePath))
  {
    FTextLocalizationManager::Get().UpdateFromLocalizationResource(locResFilePath);
    return;
  }

}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMocopiLiveLinkModule, MocopiLiveLink)
