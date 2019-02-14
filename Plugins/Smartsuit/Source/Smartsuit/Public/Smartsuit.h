// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once
//#include "ModuleManager.h"
//#include "Modules/ModuleInterface.h"
///// @cond doc_hidden
//class FSmartsuitModule : public IModuleInterface
//{
//public:
//
//	virtual void StartupModule() override;
//	virtual void ShutdownModule() override;
//};
/// @endcond
// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class ISmartsuitModule : public IModuleInterface
{
public:
	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline ISmartsuitModule& Get()
	{
		return FModuleManager::LoadModuleChecked< ISmartsuitModule >("Smartsuit");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("Smartsuit");
	}
};
