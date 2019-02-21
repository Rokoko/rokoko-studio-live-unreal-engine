// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiveLinkSourceFactory.h"
#include "VirtualProductionSourceFactory.generated.h"

class SVirtualProductionSourceEditor;
//class SLiveLinkMessageBusSourceEditor;

UCLASS()
class UVirtualProductionSourceFactory : public ULiveLinkSourceFactory
{
	GENERATED_BODY()
	
	virtual FText GetSourceDisplayName() const;
	virtual FText GetSourceTooltip() const;

	virtual TSharedPtr<SWidget> CreateSourceCreationPanel();
	virtual TSharedPtr<ILiveLinkSource> OnSourceCreationPanelClosed(bool bMakeSource);

	TSharedPtr<SVirtualProductionSourceEditor> ActiveSourceEditor;
};
