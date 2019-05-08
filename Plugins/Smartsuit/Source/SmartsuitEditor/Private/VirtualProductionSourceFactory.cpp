// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionSourceFactory.h"
#include "VirtualProductionSource.h"
#include "VirtualProductionSourceEditor.h"
#include "LiveLinkMessageBusFinder.h"

#define LOCTEXT_NAMESPACE "VirtualProductionSourceFactory"

FText UVirtualProductionSourceFactory::GetSourceDisplayName() const
{
	return LOCTEXT("SourceDisplayName", "Rokoko Studio Source");
}

FText UVirtualProductionSourceFactory::GetSourceTooltip() const
{
	return LOCTEXT("SourceTooltip", "Creates a connection to a Rokoko Studio Source");
}

TSharedPtr<SWidget> UVirtualProductionSourceFactory::CreateSourceCreationPanel()
{
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

	if (bMakeSource && ActiveSourceEditor.IsValid())
	{
		FProviderPollResultPtr Result = ActiveSourceEditor->GetSelectedSource();
		if (Result.IsValid())
		{
			NewSource = MakeShared<FVirtualProductionSource>(FText::FromString(Result->Name), FText::FromString(Result->MachineName), Result->Address);
		}
	}
	ActiveSourceEditor = nullptr;
	return NewSource;
}

#undef LOCTEXT_NAMESPACE