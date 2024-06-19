// 2023 Copyright Sony Corporation, Inc. All Rights Reserved

#include "SMocopiLiveLinkSourceFactory.h"

#include "Misc/MessageDialog.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/input/SSegmentedControl.h"
#include "Widgets/input/SButton.h"

#include "SonyMotionFormat.h"

#define LOCTEXT_NAMESPACE "MocopiLiveLinkModule"

std::unordered_map<uint16, FName> SMocopiLiveLinkSourceFactory::mPortToSubjectNameMap;

SMocopiLiveLinkSourceFactory::~SMocopiLiveLinkSourceFactory()
{
}

void SMocopiLiveLinkSourceFactory::Construct(const FArguments& Args)
{
  CreateClicked = Args._OnCreateClicked;

  ChildSlot
  [
    SNew(SVerticalBox)
    + SVerticalBox::Slot()
      .Padding(5)
      .FillHeight(1.1)
      [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
          .HAlign(HAlign_Left)
          .FillWidth(0.6f)
          .VAlign(VAlign_Center)
        [
          SNew(STextBlock)
          .Text(LOCTEXT("InputPort", "Port Number"))
        ]
        + SHorizontalBox::Slot()
          .HAlign(HAlign_Right)
          .VAlign(VAlign_Center)
          .FillWidth(0.5f)
        [
          SAssignNew(mInputPortField, SEditableTextBox)
          .Text(FText::FromString(GetSuggestedPort()))
        ]
      ]

    + SVerticalBox::Slot()
      .FillHeight(1.4)
      [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
          .HAlign(HAlign_Left)
          .Padding(5)
          .AutoWidth()
          .VAlign(VAlign_Center)
          [
            SNew(STextBlock)
            .Text(LOCTEXT("SubjectName", "Subject Name"))
          ]

        + SHorizontalBox::Slot()
          .Padding(5,0)
          .HAlign(HAlign_Right)
          .AutoWidth()
          .VAlign(VAlign_Center)
        [
          SAssignNew(mSubjectNameField, SEditableTextBox)
          .Text(FText::FromString(GetSuggestedSkeletonName()))
        ]

      ]
      
    + SVerticalBox::Slot()
      .FillHeight(1.1)
      .Padding(5)
      .HAlign(HAlign_Center)
      [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
          [
            SNew(SButton)
            .OnClicked(this, &SMocopiLiveLinkSourceFactory::OnCreateClicked)
            [
              SNew(STextBlock)
              .Text(LOCTEXT("CreateSourceButtonLabel", "Create Mocopi Source"))
            ]
          ]
      ]
  ];
}

FReply SMocopiLiveLinkSourceFactory::OnCreateClicked()
{

  if (!SonyMotionFormat::IsValid())
  {
    FText invalidSMFTitle = LOCTEXT("errorTitleInvalidSMF", "Error: SonyMotionFormat Library");
    FMessageDialog::Debugf(LOCTEXT("errorInvalidSMF", "Error loading the SonyMotionFormat Library. Please reinstall the plugin to repair the files."), &invalidSMFTitle);
    return FReply::Unhandled();
  }

  TSharedPtr<SEditableTextBox> portEditableTextPin = mInputPortField.Pin();
  TSharedPtr<SEditableTextBox> subjectNameTextPin = mSubjectNameField.Pin();

  if (portEditableTextPin && subjectNameTextPin)
  {
    FText invalidPortTitle = LOCTEXT("errorTitleInvalidPort", "Error: Invalid Port");
    
    FString portString = portEditableTextPin->GetText().ToString();
    if (!portString.IsNumeric())
    {
      FMessageDialog::Debugf(LOCTEXT("errorInvalidPort", "Invalid port.\nPlease enter a port number between 0 and 65535"), &invalidPortTitle);
      return FReply::Unhandled();
    }

    uint64 portOverflowCheck = FCString::Atoi(*portString);
    if (portOverflowCheck > 65535)
    {
      FMessageDialog::Debugf(LOCTEXT("errorInvalidPort", "Invalid port.\nPlease enter a port number between 0 and 65535"), &invalidPortTitle);
      return FReply::Unhandled();
    }

    uint16 port = uint16(portOverflowCheck);
    if (mPortToSubjectNameMap.contains(port))
    {
      FText portInUseTitle = LOCTEXT("errorTitleUsedPort", "Error: Port in Use");
      FMessageDialog::Debugf(LOCTEXT("errorUsedPort", "This port is already being used by another Mocopi LiveLink Source"), &portInUseTitle);
      return FReply::Unhandled();
    }
   
    FString subjectNameStr = subjectNameTextPin->GetText().ToString();
    if (subjectNameStr.IsEmpty())
    {
      FText emptySubjectTitle = LOCTEXT("errorTitleNoSubjectName", "Error: Empty Subject Name");
      FMessageDialog::Debugf(LOCTEXT("errorNoSubjectName", "Enter a subject name to create a Mocopi LiveLink Source"), &emptySubjectTitle);
      return FReply::Unhandled();
    }

    FName subjectName = FName(*subjectNameStr);

    CreateClicked.ExecuteIfBound(port, subjectName);
    return FReply::Handled();
  }

  return FReply::Unhandled();
}

void SMocopiLiveLinkSourceFactory::RemoveSubject(uint16 inputPort)
{
  mPortToSubjectNameMap.erase(inputPort);
}

void SMocopiLiveLinkSourceFactory::AddSubject(uint16 inputPort, FName subjectName)
{
  mPortToSubjectNameMap[inputPort] = subjectName;
}

FString SMocopiLiveLinkSourceFactory::GetSuggestedSkeletonName()
{
  FString suggestedName = DEFAULT_SKELETON_NAME;

  int count = mPortToSubjectNameMap.size();
  if (count)
  {
    int humanReadableCount = count + 1;
    suggestedName += FString::FromInt(humanReadableCount);
  }

  return suggestedName;
}

FString SMocopiLiveLinkSourceFactory::GetSuggestedPort()
{
  int port = DEFAULT_MOCOPI_PORT;

  int count = mPortToSubjectNameMap.size();
  if (count)
  {
    port += count;
  }

  return FString::FromInt(port);
}

#undef LOCTEXT_NAMESPACE
