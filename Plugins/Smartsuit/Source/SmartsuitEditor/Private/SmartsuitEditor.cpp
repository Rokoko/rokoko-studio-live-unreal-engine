// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitEditor.h"
#include "SmartsuitEditorPrivatePCH.h"

#define LOCTEXT_NAMESPACE "FSmartsuitEditorModule"

void FSmartsuitEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSmartsuitEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSmartsuitEditorModule, SmartsuitEditor)