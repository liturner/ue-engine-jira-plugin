// Copyright TD Technologies. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/UnrealString.h"
#include "GameFramework/Actor.h"
#include "UObject/Class.h"
#include "UObject/Object.h"
#include "UObject/UnrealType.h"
#include "UObject/ObjectMacros.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

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

		//UFUNCTION(BlueprintImplementableEvent, Category = "Jira")
		//void OnResponseRecieved(const FString& Request, bool bWasSuccessful);

		/**
		* Create a request that can be then called using ProcessRequest(FHttpRequestRef). Use this instead of FHttpModule::Get().CreateRequest(); so we can centrally change the HTTP Manager stuff is needed.
		*/
		FHttpRequestRef CreateRequest();

		/**
		* Process a request using the connection information stored in this instance of a Jira Connection. Note, we automatically prepend the HTTP Server address to the Request. The caller must only the path after the server specify.
		*/
		bool ProcessRequest(FHttpRequestRef HttpRequestRef);
};
