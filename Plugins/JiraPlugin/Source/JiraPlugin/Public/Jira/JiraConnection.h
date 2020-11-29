// Copyright TD Technologies. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/UnrealString.h"
#include "GameFramework/Actor.h"
#include "UObject/Class.h"
#include "UObject/Object.h"
#include "UObject/UnrealType.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectBaseUtility.h"
#include "Components/SceneComponent.h"

#include "JiraConnection.generated.h"

UCLASS(BlueprintType, CollapseCategories, AutoExpandCategories = ("Authentication", "Connection"), HideCategories = ("LOD", "Input", "Collision"), meta=(ShortTooltip="A Jira Connection is an object which holds connection information for, and communicates with, a Jira instance.", ToolTip = "A Jira Connection is an object which holds connection information for, and communicates with, a Jira instance."))
class JIRAPLUGIN_API AJiraConnection : public AActor
{
		GENERATED_BODY()

public:
		// Sets default values for this actor's properties
		AJiraConnection();

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Authentication")
		FString UserEmail;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Authentication")
		FString ApiToken;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connection")
		FString ServerUrl;

		UFUNCTION(BlueprintPure, Category = "Jira")
		static bool CanAuthenticate(const AJiraConnection* JiraConnection);

		UFUNCTION(BlueprintImplementableEvent, Category = "Jira")
		void OnResponseRecieved(const FString& Request, bool bWasSuccessful);


};
