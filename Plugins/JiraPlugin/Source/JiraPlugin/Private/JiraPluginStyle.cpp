// Copyright Epic Games, Inc. All Rights Reserved.

#include "JiraPluginStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr< FSlateStyleSet > FJiraPluginStyle::StyleInstance = NULL;

void FJiraPluginStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FJiraPluginStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FJiraPluginStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("JiraPluginStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon64x64(64.0f, 64.0f);
const FVector2D Icon128x128(128.0f, 128.0f);

TSharedRef< FSlateStyleSet > FJiraPluginStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("JiraPluginStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("JiraPlugin")->GetBaseDir() / TEXT("Resources"));

	Style->Set("JiraPlugin.OpenPluginWindow", new IMAGE_BRUSH(TEXT("JiraThumbnail_40x"), Icon40x40));
	Style->Set("ClassThumbnail.JiraConnection", new IMAGE_BRUSH(TEXT("JiraThumbnail_16x"), Icon16x16));
	Style->Set("ClassThumbnail.JiraConnection", new IMAGE_BRUSH(TEXT("JiraThumbnail_20x"), Icon20x20));
	Style->Set("ClassThumbnail.JiraConnection", new IMAGE_BRUSH(TEXT("JiraThumbnail_64x"), Icon64x64));
	Style->Set("ClassIcon.JiraConnection", new IMAGE_BRUSH(TEXT("JiraIcon_16x"), Icon16x16));
	Style->Set("ClassIcon.JiraConnection", new IMAGE_BRUSH(TEXT("JiraIcon_20x"), Icon20x20));

	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FJiraPluginStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FJiraPluginStyle::Get()
{
	return *StyleInstance;
}
