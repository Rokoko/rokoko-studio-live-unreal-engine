// Fill out your copyright notice in the Description page of Project Settings.

/**
 *
class VirtualProductionSourceEditor
{
public:
	VirtualProductionSourceEditor();
	~VirtualProductionSourceEditor();
}; 
 */


#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "IMessageContext.h"
#include "MessageEndpoint.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "VPStreamingNetwork.h"
#include "SmartsuitReceiver.h"
#include "VirtualProductionFrame.h"
#include "Misc/Guid.h"
#include "LiveLink/Public/LiveLinkMessageBusFinder.h"

//struct FLiveLinkPongMessage;
struct FMessageAddress;
struct FProviderPollResult;
class ITableRow;
class STableViewBase;

//typedef TSharedPtr<FProviderPollResult> FProviderPollResultPtr;

DECLARE_DELEGATE_OneParam(FOnVirtualProductionSourceSelected, FProviderPollResultPtr);

class SVirtualProductionSourceEditor : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SVirtualProductionSourceEditor){}
		SLATE_EVENT(FOnVirtualProductionSourceSelected, OnSourceSelected)
	SLATE_END_ARGS()

		~SVirtualProductionSourceEditor();

	void Construct(const FArguments& Args);

	FProviderPollResultPtr GetSelectedSource() const { 
		
		UE_LOG(LogTemp, Display, TEXT("GetSelectedSource %d"), SelectedResult.IsValid());
		return SelectedResult;
	}

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);

private:

	//void HandlePongMessage(const FLiveLinkPongMessage& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);

	TSharedRef<ITableRow> MakeSourceListViewWidget(FProviderPollResultPtr PollResult, const TSharedRef<STableViewBase>& OwnerTable) const;

	void OnSourceListSelectionChanged(FProviderPollResultPtr PollResult, ESelectInfo::Type SelectionType);

	TSharedPtr<SListView<FProviderPollResultPtr>> ListView;

	TArray<FProviderPollResultPtr> PollData;

	FProviderPollResultPtr SelectedResult;

	FOnVirtualProductionSourceSelected OnSourceSelected;

	//TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> MessageEndpoint;

	FGuid CurrentPollRequest;

	// Time since our UI was last ticked, allow us to refresh if we haven't been onscreen for a while
	double LastTickTime;

	ASmartsuitReceiver *receiver;
	VPStreamingNetwork *VPnet;
	FVirtualProductionFrame *VPFrame;
};