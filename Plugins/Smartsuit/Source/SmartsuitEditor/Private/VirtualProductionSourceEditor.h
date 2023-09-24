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
#include "VirtualProductionFrame.h"
#include "Misc/Guid.h"
#include "LiveLinkMessageBusFinder.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

struct FMessageAddress;
struct FProviderPollResult;
class ITableRow;
class STableViewBase;
class SEditableTextBox;

struct SCreationInfo
{
	FIPv4Endpoint	m_Address;
	bool m_UseLZ4Compression; // TODO:
};


class SVirtualProductionSourceEditor : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_TwoParams(FOnVirtualProductionSourceSelected, SCreationInfo, FProviderPollResultPtr);

	SLATE_BEGIN_ARGS(SVirtualProductionSourceEditor){}
		SLATE_EVENT(FOnVirtualProductionSourceSelected, OnSourceSelected)
	SLATE_END_ARGS()

	virtual ~SVirtualProductionSourceEditor();

	void Construct(const FArguments& Args);

	FProviderPollResultPtr GetSelectedSource() const { 
		
		UE_LOG(LogTemp, Display, TEXT("GetSelectedSource %d"), SelectedResult.IsValid());
		return SelectedResult;
	}

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);

private:
	void OnEndpointChanged(const FText& NewValue, ETextCommit::Type);

	TWeakPtr<SEditableTextBox>		m_NetworkAddress;

	TSharedRef<ITableRow> MakeSourceListViewWidget(FProviderPollResultPtr PollResult, const TSharedRef<STableViewBase>& OwnerTable) const;

	void OnSourceListSelectionChanged(FProviderPollResultPtr PollResult, ESelectInfo::Type SelectionType);

	TSharedPtr<SListView<FProviderPollResultPtr>> ListView;

	TArray<FProviderPollResultPtr> PollData;

	FProviderPollResultPtr SelectedResult;

	FOnVirtualProductionSourceSelected OnSourceSelected;

	FGuid CurrentPollRequest;

	// Time since our UI was last ticked, allow us to refresh if we haven't been onscreen for a while
	double LastTickTime;

	FVirtualProductionFrame *VPFrame;
};