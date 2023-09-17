// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionSourceEditor.h"
#include "VirtualProductionSource.h"
#include "LiveLinkMessageBusFinder.h"
#include "Widgets/Layout/SBox.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
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
}

void SVirtualProductionSourceEditor::Construct(const FArguments& Args)
{
	OnSourceSelected = Args._OnSourceSelected;
	LastTickTime = 0.0;

	// default values
	FIPv4Address address(FIPv4Address::Any);

	FIPv4Endpoint Endpoint;
	Endpoint.Address = address;
	Endpoint.Port = 14043;

	ChildSlot
		[
			SNew(SBox)
			.HeightOverride(200)
			.WidthOverride(200)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.FillWidth(0.5f)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("UDPAddress", "Network Address:Port"))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.FillWidth(0.5f)
					[
						SAssignNew(m_NetworkAddress, SEditableTextBox)
						.Text(FText::FromString(Endpoint.ToString()))
						.OnTextCommitted(this, &SVirtualProductionSourceEditor::OnEndpointChanged)
					]
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
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
			]
		];
}

void SVirtualProductionSourceEditor::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	const double CurrentTickTime = FPlatformTime::Seconds();
	if (CurrentTickTime - LastTickTime > 1.f)
	{
		bool bIsValidProvider = true;
		#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 24)
		FProviderPollResult *result = new FProviderPollResult(FMessageAddress::NewAddress(), FString("Studio"), FString(""), 0.0, bIsValidProvider);
		#else
		FProviderPollResult* result = new FProviderPollResult(FMessageAddress::NewAddress(), FString("Studio"), FString(""));
		#endif
		FProviderPollResultPtr *poll = new FProviderPollResultPtr(result);
		PollData.Empty();
		if (!FVirtualProductionSource::Get()) {
			PollData.Add(*poll);
		}
		
		ListView->RequestListRefresh();

		CurrentPollRequest = FGuid::NewGuid();
	}
	LastTickTime = CurrentTickTime;
}

TSharedRef<ITableRow> SVirtualProductionSourceEditor::MakeSourceListViewWidget(FProviderPollResultPtr PollResult, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SProviderPollRow, OwnerTable).PollResultPtr(PollResult);
}

void SVirtualProductionSourceEditor::OnSourceListSelectionChanged(FProviderPollResultPtr PollResult, ESelectInfo::Type SelectionType)
{
	SelectedResult = PollResult;

	TSharedPtr<SEditableTextBox> address = m_NetworkAddress.Pin();
	if (address.IsValid())
	{
		FIPv4Endpoint Endpoint;
		if (FIPv4Endpoint::Parse(address->GetText().ToString(), Endpoint))
		{
			SCreationInfo info{
				Endpoint
			};

			OnSourceSelected.ExecuteIfBound(info, SelectedResult);
		}
	}
}

void SVirtualProductionSourceEditor::OnEndpointChanged(const FText& NewValue, ETextCommit::Type)
{
	TSharedPtr<SEditableTextBox> address = m_NetworkAddress.Pin();
	if (address.IsValid())
	{
		FIPv4Endpoint Endpoint;
		if (!FIPv4Endpoint::Parse(NewValue.ToString(), Endpoint))
		{
			Endpoint.Address = FIPv4Address::Any;
			Endpoint.Port = 14043;
			address->SetText(FText::FromString(Endpoint.ToString()));
		}
	}
}

#undef LOCTEXT_NAMESPACE