// Copyright TD Technologies. All Rights Reserved.

#include "JiraPlugin.h"
#include "JiraPluginStyle.h"
#include "JiraTypes.h"
#include "JiraConnection.h"
#include "Modules/ModuleManager.h"
#include "HttpModule.h"

#define LOCTEXT_NAMESPACE "FJiraPluginModule"

void FJiraPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FJiraPluginStyle::Initialize();
	FJiraPluginStyle::ReloadTextures();
	
	FModuleManager::LoadModuleChecked<FHttpModule>("HTTP");
}

void FJiraPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FJiraPluginStyle::Shutdown();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FJiraPluginModule, JiraPlugin)