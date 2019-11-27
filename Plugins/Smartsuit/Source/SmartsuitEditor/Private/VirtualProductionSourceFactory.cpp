// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionSourceFactory.h"
#include "VirtualProductionSource.h"
#include "VirtualProductionSourceEditor.h"
#include "LiveLinkMessageBusFinder.h"
#include "Features/IModularFeatures.h"

#define LOCTEXT_NAMESPACE "VirtualProductionSourceFactory"

FText UVirtualProductionSourceFactory::GetSourceDisplayName() const
{
	return LOCTEXT("SourceDisplayName", "Rokoko Studio Source");
}

FText UVirtualProductionSourceFactory::GetSourceTooltip() const
{
	return LOCTEXT("SourceTooltip", "Creates a connection to a Rokoko Studio Source");
}

TSharedPtr<SWidget> UVirtualProductionSourceFactory::BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const
{
	return SNew(SVirtualProductionSourceEditor)
		.OnSourceSelected(FOnVirtualProductionSourceSelected::CreateUObject(this, &UVirtualProductionSourceFactory::OnSourceSelected, OnLiveLinkSourceCreated));
}

UVirtualProductionSourceFactory::EMenuType UVirtualProductionSourceFactory::GetMenuType() const
{
	if (IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		ILiveLinkClient& LiveLinkClient = IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
		auto livelink = FVirtualProductionSource::Get();
		
		if (livelink && !livelink.IsValid() || !LiveLinkClient.HasSourceBeenAdded(livelink))
		{
			return EMenuType::SubPanel;
		}
	}
	return EMenuType::Disabled;
}

void UVirtualProductionSourceFactory::OnSourceSelected(FProviderPollResultPtr SelectedSource, FOnLiveLinkSourceCreated InOnLiveLinkSourceCreated) const
{
	if (SelectedSource.IsValid())
	{
#if WITH_EDITOR
		bool bDoesAlreadyExist = false;
		{
			ILiveLinkClient& LiveLinkClient = IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
			TArray<FGuid> Sources = LiveLinkClient.GetSources();
			for (FGuid SourceGuid : Sources)
			{
				if (LiveLinkClient.GetSourceType(SourceGuid).ToString() == SelectedSource->Name)
				{
					bDoesAlreadyExist = true;
					break;
				}
			}
		}

		if (bDoesAlreadyExist)
		{
			if (EAppReturnType::No == FMessageDialog::Open(EAppMsgType::YesNo, EAppReturnType::Yes, LOCTEXT("AddSourceWithSameName", "This provider name already exist. Are you sure you want to add a new one?")))
			{
				return;
			}
		}
#endif

		TSharedPtr<FVirtualProductionSource> SharedPtr = MakeShared<FVirtualProductionSource>(FText::FromString(SelectedSource->Name), FText::FromString(SelectedSource->MachineName), SelectedSource->Address);
		FVirtualProductionSource::SetInstance(SharedPtr);
		FString ConnectionString = FString::Printf(TEXT("Name=\"%s\""), *SelectedSource->Name);
		InOnLiveLinkSourceCreated.ExecuteIfBound(SharedPtr, MoveTemp(ConnectionString));
	}
}

TSharedPtr<ILiveLinkSource> UVirtualProductionSourceFactory::CreateSource(const FString& ConnectionString) const
{
	FString Name;
	if (!FParse::Value(*ConnectionString, TEXT("Name="), Name))
	{
		return TSharedPtr<ILiveLinkSource>();
	}

	return MakeShared<FVirtualProductionSource>(FText::FromString(Name), FText::GetEmpty(), FMessageAddress());
}

#undef LOCTEXT_NAMESPACE