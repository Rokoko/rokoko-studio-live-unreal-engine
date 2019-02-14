// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "Smartsuit.h"

class FSmartsuitModule : public ISmartsuitModule
{
	virtual void StartupModule() override
	{
	
	}

	virtual void ShutdownModule() override
	{
	
	}
};

IMPLEMENT_MODULE(FSmartsuitModule, Smartsuit);
