// Copyright Epic Games, Inc. All Rights Reserved.

#include "JiraPlugin.h"
#include "JiraPluginStyle.h"
#include "JiraPluginCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Misc/Base64.h"
#include "Interfaces/IHttpResponse.h"
#include "Jira/JiraTypes.h"
#include "Jira/JiraConnection.h"

static const FName JiraPluginTabName("JiraPlugin");

#define LOCTEXT_NAMESPACE "FJiraPluginModule"

void FJiraPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FJiraPluginStyle::Initialize();
	FJiraPluginStyle::ReloadTextures();

	FJiraPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FJiraPluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FJiraPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FJiraPluginModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(JiraPluginTabName, FOnSpawnTab::CreateRaw(this, &FJiraPluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FJiraPluginTabTitle", "Jira"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	FModuleManager::LoadModuleChecked<FHttpModule>("HTTP");
}

void FJiraPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FJiraPluginStyle::Shutdown();

	FJiraPluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(JiraPluginTabName);
}

int aGlobalInt = 0;

void FJiraPluginModule::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) 
{
	int32 ResponseCode = Response->GetResponseCode();
	FString ResponseText = Response->GetContentAsString();

	FJiraPageBeanProject projects;
	if (!projects.FromJson(ResponseText))
	{
		// error
	}

	bool hasNextPage = projects.HasNextPage();
	if (!hasNextPage && aGlobalInt == 5)
	{
		// error
		aGlobalInt = ResponseCode;
	}

	if (projects.HasMaxResults())
	{
		aGlobalInt = 14;
	}

}

TSharedRef<SDockTab> FJiraPluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FHttpRequestRef getProjectRequest = FHttpModule::Get().CreateRequest();
	getProjectRequest->SetVerb("GET");
	getProjectRequest->AppendToHeader("Authorization", "Basic " + FBase64::Encode("luke.turner@tdtek.de:7YVZF0f5pprkBvQyU7pWD190"));
	getProjectRequest->AppendToHeader("Accept", "application/json");
	getProjectRequest->SetURL("https://tdtek.atlassian.net/rest/api/3/project/search");
	getProjectRequest->OnProcessRequestComplete().BindRaw(this, &FJiraPluginModule::OnResponseReceived);
	getProjectRequest->ProcessRequest();

	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FJiraPluginModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("JiraPlugin.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FJiraPluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(JiraPluginTabName);
}

void FJiraPluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FJiraPluginCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FJiraPluginCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FJiraPluginModule, JiraPlugin)