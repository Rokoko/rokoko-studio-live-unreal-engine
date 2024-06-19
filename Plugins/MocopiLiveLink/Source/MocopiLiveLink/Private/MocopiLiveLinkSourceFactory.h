// 2023 Copyright Sony Corporation, Inc. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "LiveLinkSourceFactory.h"
#include "SMocopiLiveLinkSourceFactory.h"
#include "MocopiLiveLinkSourceFactory.generated.h"


UCLASS()
class UMocopiLiveLinkSourceFactory : public ULiveLinkSourceFactory
{
public:
  GENERATED_BODY()

  virtual FText GetSourceDisplayName() const;
  virtual FText GetSourceTooltip() const;

  virtual EMenuType GetMenuType() const override { return EMenuType::SubPanel; }
  virtual TSharedPtr<SWidget> BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const override;

  virtual TSharedPtr<ILiveLinkSource> CreateSource(const FString& ConnectionString) const override;

private:

  // Creates the Mocopi LiveLinkSource
  void OnCreateClicked(uint16 inputPort, FName subjectName, FOnLiveLinkSourceCreated onLiveLinkSourceCreated) const;
};
