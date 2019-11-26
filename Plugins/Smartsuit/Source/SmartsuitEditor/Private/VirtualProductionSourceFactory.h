// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LiveLinkSourceFactory.h"
#include "LiveLinkMessageBusFinder.h"
#include "VirtualProductionSourceFactory.generated.h"

class SVirtualProductionSourceEditor;

UCLASS()
class UVirtualProductionSourceFactory : public ULiveLinkSourceFactory
{
	GENERATED_BODY()
	
	virtual FText GetSourceDisplayName() const;
	virtual FText GetSourceTooltip() const;

	virtual TSharedPtr<SWidget> BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const override;
	virtual EMenuType GetMenuType() const override;
	virtual TSharedPtr<ILiveLinkSource> CreateSource(const FString& ConnectionString) const override;

private:
	void OnSourceSelected(FProviderPollResultPtr SelectedSource, FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const;
};
