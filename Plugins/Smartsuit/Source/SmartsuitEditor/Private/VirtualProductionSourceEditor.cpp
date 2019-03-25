// Fill out your copyright notice in the Description page of Project Settings.

#include "VirtualProductionSourceEditor.h"
#include "LiveLinkMessageBusFinder.h"
#include "LiveLinkEditor/Private/LiveLinkClientPanel.h"
#include "Widgets/Layout/SBox.h"

#include "MessageEndpointBuilder.h"

#define LOCTEXT_NAMESPACE "VirtualProductionSourceEditor"

namespace ProviderPollUI
{
	const FName TypeColumnName(TEXT("Type"));
	const FName MachineColumnName(TEXT("Machine"));
};

class SProviderPollRow : public SMultiColumnTableRow<FProviderPollResultPtr>
{
public:
	SLATE_BEGIN_ARGS(SProviderPollRow) {}

	/** The list item for this row */
		SLATE_ARGUMENT(FProviderPollResultPtr, PollResultPtr)

	SLATE_END_ARGS()

	void Construct(const FArguments& Args, const TSharedRef<STableViewBase>& OwnerTableView)
	{
			UE_LOG(LogTemp, Display, TEXT("Construct2"));
			PollResultPtr = Args._PollResultPtr;

		SMultiColumnTableRow<FProviderPollResultPtr>::Construct(
			FSuperRowType::FArguments()
			.Padding(1.0f),
			OwnerTableView
		);
	}

	/** Overridden from SMultiColumnTableRow.  Generates a widget for this column of the list view. */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
	{
		UE_LOG(LogTemp, Display, TEXT("GenerateWidgetForColumn"));
		if (ColumnName == ProviderPollUI::TypeColumnName)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(PollResultPtr->Name));
		}
		else if (ColumnName == ProviderPollUI::MachineColumnName)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(PollResultPtr->MachineName));
		}

		return SNullWidget::NullWidget;
	}

private:

	FProviderPollResultPtr PollResultPtr;
};


SVirtualProductionSourceEditor::~SVirtualProductionSourceEditor()
{
	//FMessageEndpoint::SafeRelease(MessageEndpoint);
}

void SVirtualProductionSourceEditor::Construct(const FArguments& Args)
{
	UE_LOG(LogTemp, Display, TEXT("Construct"));
	LastTickTime = 0.0;

	//MessageEndpoint = FMessageEndpoint::Builder(TEXT("VirtualProductionSource"))
	//	.Handling<FLiveLinkPongMessage>(this, &SVirtualProductionSourceEditor::HandlePongMessage);
	
	ChildSlot
		[
			SNew(SBox)
			.HeightOverride(200)
		.WidthOverride(200)
		[
			SAssignNew(ListView, SListView<FProviderPollResultPtr>)
			.ListItemsSource(&PollData)
		.SelectionMode(ESelectionMode::SingleToggle)
		.OnGenerateRow(this, &SVirtualProductionSourceEditor::MakeSourceListViewWidget)
		.OnSelectionChanged(this, &SVirtualProductionSourceEditor::OnSourceListSelectionChanged)
		.HeaderRow
		(
			SNew(SHeaderRow)
			+ SHeaderRow::Column(ProviderPollUI::TypeColumnName)
			.FillWidth(43.0f)
			.DefaultLabel(LOCTEXT("TypeColumnHeaderName", "Source Type"))
			+ SHeaderRow::Column(ProviderPollUI::MachineColumnName)
			.FillWidth(43.0f)
			.DefaultLabel(LOCTEXT("MachineColumnHeaderName", "Source Machine"))
		)
		]
		];
}

void SVirtualProductionSourceEditor::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	const double CurrentTickTime = FPlatformTime::Seconds();
	if (CurrentTickTime - LastTickTime > 1.f)
	{
		FProviderPollResult *result = new FProviderPollResult(FMessageAddress::NewAddress(), FString("Studio"), FString(""));
		FProviderPollResultPtr *poll = new FProviderPollResultPtr(result);


		PollData.Add(*poll);
		ListView->RequestListRefresh();

		CurrentPollRequest = FGuid::NewGuid();

		//MessageEndpoint->Publish(new FLiveLinkPingMessage(CurrentPollRequest));
		FText subject = LOCTEXT("Studio", "Studio");
		//SLiveLinkClientPanel::HandleAddVirtualSubject(subject, ETextCommit::Type::Default);

	}
	LastTickTime = CurrentTickTime;
}

TSharedRef<ITableRow> SVirtualProductionSourceEditor::MakeSourceListViewWidget(FProviderPollResultPtr PollResult, const TSharedRef<STableViewBase>& OwnerTable) const
{
	UE_LOG(LogTemp, Display, TEXT("MakeSourceListViewWidget %d"), PollResult.IsValid());
	return SNew(SProviderPollRow, OwnerTable).PollResultPtr(PollResult);
}

void SVirtualProductionSourceEditor::OnSourceListSelectionChanged(FProviderPollResultPtr PollResult, ESelectInfo::Type SelectionType)
{
	UE_LOG(LogTemp, Display, TEXT("OnSourceListSelectionChanged result %d"), PollResult.IsValid());
	SelectedResult = PollResult;
}

//void SVirtualProductionSourceEditor::HandlePongMessage(const FLiveLinkPongMessage& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
//{
//	//UE_LOG(LogTemp, Display, TEXT("HandlePongMessage"));
//	///*if (Message.PollRequest == CurrentPollRequest)
//	//{
//	//	PollData.Add(MakeShared<FProviderPollResult>(Context->GetSender(), Message.ProviderName, Message.MachineName));
//
//	//	ListView->RequestListRefresh();
//	//}*/
//}

#undef LOCTEXT_NAMESPACE