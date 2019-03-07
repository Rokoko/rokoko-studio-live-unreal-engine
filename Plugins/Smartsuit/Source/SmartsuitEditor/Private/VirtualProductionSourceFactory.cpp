// Fill out your copyright notice in the Description page of Project Settings.

#include "VirtualProductionSourceFactory.h"
#include "VirtualProductionSource.h"
//#include "LiveLinkMessageBusSource.h"
//#include "C:/Program Files/Epic Games/UE_4.21/Engine/Plugins/Animation/LiveLink/Source/LiveLinkEditor/Private/LiveLinkMessageBusSourceEditor.h"
#include "VirtualProductionSourceEditor.h"
#include "LiveLinkMessageBusFinder.h"

#define LOCTEXT_NAMESPACE "VirtualProductionSourceFactory"

FText UVirtualProductionSourceFactory::GetSourceDisplayName() const
{
	return LOCTEXT("SourceDisplayName", "Rokoko Virtual Production Source");
}

FText UVirtualProductionSourceFactory::GetSourceTooltip() const
{
	return LOCTEXT("SourceTooltip", "Creates a connection to a Rokoko Virtual Production Source");
}

TSharedPtr<SWidget> UVirtualProductionSourceFactory::CreateSourceCreationPanel()
{
	UE_LOG(LogTemp, Display, TEXT("CreateSourceCreationPanel %d"), ActiveSourceEditor.IsValid());
	if (!ActiveSourceEditor.IsValid())
	{
		SAssignNew(ActiveSourceEditor, SVirtualProductionSourceEditor);
	}
	return ActiveSourceEditor;
}

TSharedPtr<ILiveLinkSource> UVirtualProductionSourceFactory::OnSourceCreationPanelClosed(bool bMakeSource)
{
	//Clean up
	TSharedPtr<FVirtualProductionSource> NewSource = nullptr;
	UE_LOG(LogTemp, Display, TEXT("OnSourceCreationPanelClosed %d %d"), bMakeSource, ActiveSourceEditor.IsValid());

	if (bMakeSource && ActiveSourceEditor.IsValid())
	{
		FProviderPollResultPtr Result = ActiveSourceEditor->GetSelectedSource();
		UE_LOG(LogTemp, Display, TEXT("OnSourceCreationPanelClosed result %d"), Result.IsValid());
		if (Result.IsValid())
		{
			NewSource = MakeShared<FVirtualProductionSource>(FText::FromString(Result->Name), FText::FromString(Result->MachineName), Result->Address);
			UE_LOG(LogTemp, Display, TEXT("SourceType:: %s"), NewSource.Get()->GetSourceType());
		}
	}
	ActiveSourceEditor = nullptr;
	return NewSource;
}

#undef LOCTEXT_NAMESPACE