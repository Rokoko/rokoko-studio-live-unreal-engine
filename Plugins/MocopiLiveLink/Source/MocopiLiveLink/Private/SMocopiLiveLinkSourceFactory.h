// 2023 Copyright Sony Corporation, Inc. All Rights Reserved

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SEditableTextBox.h"

#include <unordered_map>

const int DEFAULT_MOCOPI_PORT = 12351;
const FString DEFAULT_SKELETON_NAME = "MocopiSkeleton";

class SMocopiLiveLinkSourceFactory : public SCompoundWidget
{
public:
  DECLARE_DELEGATE_TwoParams(FOnCreateClicked, uint16, FName);

  SLATE_BEGIN_ARGS(SMocopiLiveLinkSourceFactory) {}
    SLATE_EVENT(FOnCreateClicked, OnCreateClicked)
  SLATE_END_ARGS()

  ~SMocopiLiveLinkSourceFactory();

  void Construct(const FArguments& Args);

  static void RemoveSubject(uint16 inputPort);

  static void AddSubject(uint16 inputPort, FName subjectName);

private:

  // map of subjects(sources) created
  static std::unordered_map<uint16, FName> mPortToSubjectNameMap;

  TWeakPtr<SEditableTextBox> mInputPortField;
  TWeakPtr<SEditableTextBox> mSubjectNameField;

  FReply OnCreateClicked();

  FOnCreateClicked CreateClicked;

  FString GetSuggestedSkeletonName();
  FString GetSuggestedPort();

};
