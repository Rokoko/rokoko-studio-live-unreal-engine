// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once
//#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
/// @cond no_doc
class FSmartsuitEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

/// @nocond