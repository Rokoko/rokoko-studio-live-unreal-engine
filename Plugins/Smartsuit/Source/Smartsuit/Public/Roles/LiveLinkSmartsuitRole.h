// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "Roles/LiveLinkTransformRole.h"
#include "LiveLinkSmartsuitRole.generated.h"

/**
 * Role associated for Smartsuit data.
 */
UCLASS(BlueprintType, meta = (DisplayName = "Smartsuit Role"))
class SMARTSUIT_API ULiveLinkSmartsuitRole : public ULiveLinkTransformRole
{
	GENERATED_BODY()

public:
	//~ Begin ULiveLinkRole interface
	virtual UScriptStruct* GetStaticDataStruct() const override;
	virtual UScriptStruct* GetFrameDataStruct() const override;
	virtual UScriptStruct* GetBlueprintDataStruct() const override;

	virtual bool InitializeBlueprintData(const FLiveLinkSubjectFrameData& InSourceData, FLiveLinkBlueprintDataStruct& OutBlueprintData) const override;

	virtual FText GetDisplayName() const override;
	//~ End ULiveLinkRole interface
};
