// Copyright TD Technologies. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/CoreOnline.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Containers/UnrealString.h"
#include "Jira/JiraConnection.h"
#include "Jira/JiraTypes.h"
#include "GetProject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetProjectResult, FJiraProject, JiraProject);

UCLASS(MinimalAPI)
class UGetProjectAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UGetProjectAsync(const FObjectInitializer& ObjectInitializer);

	// Called when there is a successful query
	UPROPERTY(BlueprintAssignable)
		FGetProjectResult OnSuccess;

	// Called when there is an unsuccessful query
	UPROPERTY(BlueprintAssignable)
		FGetProjectResult OnFailure;

	// Shows the login UI for the currently active online subsystem, if the subsystem supports a login UI.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Jira")
		static UGetProjectAsync* GetProjectAsync(const FString projectIdOrKey, const AJiraConnection* JiraConnection);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	// End of UBlueprintAsyncActionBase interface

};
