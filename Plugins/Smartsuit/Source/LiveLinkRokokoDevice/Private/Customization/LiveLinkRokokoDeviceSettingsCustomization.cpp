// Copyright Rokoko. All Rights Reserved.

#include "Customization/LiveLinkRokokoDeviceSettingsCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Devices/LiveLinkRokokoDevice.h"
#include "IDetailPropertyRow.h"

#if ROKOKO_WITH_LIVELINKHUB
#include "NamingTokensSpecifiers.h"
#include "SNamingTokensEditableTextBox.h"
#endif

#define LOCTEXT_NAMESPACE "LiveLinkRokokoDevice"

TSharedRef<IDetailCustomization> FLiveLinkRokokoDeviceSettingsCustomization::MakeInstance()
{
    return MakeShared<FLiveLinkRokokoDeviceSettingsCustomization>();
}

void FLiveLinkRokokoDeviceSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Rokoko Device");

    TArray<TSharedRef<IPropertyHandle>> DefaultProperties;
    Category.GetDefaultProperties(DefaultProperties);

#if ROKOKO_WITH_LIVELINKHUB
    FNamingTokenFilterArgs FilterArgs;
    FilterArgs.bIncludeGlobal = false;
    FilterArgs.AdditionalNamespacesToInclude.Add(TEXT("llh"));
#endif

    for (const TSharedRef<IPropertyHandle>& PropertyHandle : DefaultProperties)
    {
        DetailBuilder.HideProperty(PropertyHandle);
    }

    for (TSharedRef<IPropertyHandle>& PropertyHandle : DefaultProperties)
    {
        IDetailPropertyRow& Row = Category.AddProperty(PropertyHandle);

#if ROKOKO_WITH_LIVELINKHUB
        if (!PropertyHandle->HasMetaData(*UE::NamingTokens::Specifiers::UseNamingTokens))
        {
            continue;
        }

        Row.CustomWidget()
            .NameContent()
            [
                PropertyHandle->CreatePropertyNameWidget(
                    LOCTEXT("FilenameFormatLabel", "Output Filename Format"))
            ]
            .ValueContent()
            [
                SNew(SNamingTokensEditableTextBox)
                .ShouldEvaluateTokens(false)
                .FilterArgs(FilterArgs)
                .NamespaceSuggestionPriority(TArray<FString>{ TEXT("llh") })
                .Text_Lambda([PropertyHandle]() -> FText
                {
                    FString Value;
                    PropertyHandle->GetValue(Value);
                    return FText::FromString(Value);
                })
                .OnTextChanged_Lambda([PropertyHandle](const FText& InText)
                {
                    PropertyHandle->SetValue(InText.ToString());
                })
            ];
        #else
            (void)Row;
        #endif
    }
}

#undef LOCTEXT_NAMESPACE
