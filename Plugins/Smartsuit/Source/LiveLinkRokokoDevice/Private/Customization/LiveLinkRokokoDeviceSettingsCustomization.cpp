// Copyright Rokoko. All Rights Reserved.

#include "Customization/LiveLinkRokokoDeviceSettingsCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Devices/LiveLinkRokokoDevice.h"
#include "IDetailPropertyRow.h"
#include "NamingTokensSpecifiers.h"
#include "SNamingTokensEditableTextBox.h"

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

    FNamingTokenFilterArgs FilterArgs;
    FilterArgs.bIncludeGlobal = false;
    FilterArgs.AdditionalNamespacesToInclude.Add(TEXT("llh"));

    for (const TSharedRef<IPropertyHandle>& PropertyHandle : DefaultProperties)
    {
        DetailBuilder.HideProperty(PropertyHandle);
    }

    for (TSharedRef<IPropertyHandle>& PropertyHandle : DefaultProperties)
    {
        IDetailPropertyRow& Row = Category.AddProperty(PropertyHandle);

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
    }
}

#undef LOCTEXT_NAMESPACE
