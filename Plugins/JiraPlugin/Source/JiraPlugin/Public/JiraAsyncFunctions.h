// Copyright TD Technologies. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/CoreOnline.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "JiraConnection.h"
#include "JiraTypes.h"

#include "JiraAsyncFunctions.generated.h"


#define BEGIN_JIRA_ERROR_CODES \
private: \
	const TMap<int32, FString> SupportedErrorsInitialiser() \
	{ \
		TMap<int32, FString> SupportedErrorsMap;

#define JIRA_ERROR_CODE(Code, Description) \
		SupportedErrorsMap.Add(Code, TEXT(Description));

#define END_JIRA_ERROR_CODES \
		return SupportedErrorsMap; \
	} \
	const TMap<int32, FString> ErrorMap = SupportedErrorsInitialiser();


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetProjectResult, FJiraError, Error, FJiraProject, JiraProject);

UCLASS(MinimalAPI)
class UGetProjectAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	BEGIN_JIRA_ERROR_CODES
		JIRA_ERROR_CODE(201, "Grapefruit");
		JIRA_ERROR_CODE(404, "Pineapple");
	END_JIRA_ERROR_CODES

public:
	UGetProjectAsync(const FObjectInitializer& ObjectInitializer);

	// Called when the operation is successfull
	UPROPERTY(BlueprintAssignable)
		FGetProjectResult OnSuccess;

	// Called when Jira responds with anything other than "OK"
	UPROPERTY(BlueprintAssignable)
		FGetProjectResult OnFailure;

	// Returns the project details for a project.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Jira")
		static UGetProjectAsync* GetProjectAsync(const FString ProjectIdOrKey, UPARAM(ref)AJiraConnection* JiraConnection);

	virtual void Activate() override;

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	TWeakObjectPtr<AJiraConnection> JiraConnectionWeakPtr;

	FString ProjectIdOrKey;
};
