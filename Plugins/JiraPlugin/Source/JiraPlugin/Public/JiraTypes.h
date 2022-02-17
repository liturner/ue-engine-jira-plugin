// Copyright TD Technologies. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "Containers/UnrealString.h"
#include "Serialization/JsonSerializerMacros.h"
#include "Dom/JsonValue.h"
#include "UObject/UnrealType.h"

#include "JiraTypes.generated.h"

#define INT32_DEFAULT int32()
#define INT64_DEFAULT int64()

USTRUCT(BlueprintType)
struct JIRAPLUGIN_API FJiraError
#if CPP
	: FJsonSerializable
#endif
{
	GENERATED_BODY()

#if CPP
public:
	BEGIN_JSON_SERIALIZER
	END_JSON_SERIALIZER
#endif
public:

	/**
	* Default constructor will initialise an empty structure with a "200" response code
	*/
	FJiraError();

	/**
	* The HTTP response code which Jira returned.
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "HTTP")
	int32 ResponseCode;

	/**
	* A short description of the error which has occured. Further information may be available.
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "General")
	FString ErrorBrief;
};


USTRUCT(BlueprintType)
struct JIRAPLUGIN_API FJiraIssueTypeDetails
#if CPP
	: FJsonSerializable
#endif
{
	GENERATED_BODY()

#if CPP
public:
	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("self", SelfJira);
		JSON_SERIALIZE("id", ID);
		JSON_SERIALIZE("description", Description);
		JSON_SERIALIZE("iconUrl", IconUrl);
		JSON_SERIALIZE("name", Name);
		JSON_SERIALIZE("subtask", bIsSubtask);
		JSON_SERIALIZE("avatartId", AvatarID);
		JSON_SERIALIZE("entityId", EntityID);
		JSON_SERIALIZE("heirarchyLevel", HeirarchyLevel);
	END_JSON_SERIALIZER
#endif
public:

	/**
	* The URL of the project details.
	* Format: uri
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HTTP", meta = (DisplayName = "Self"))
	FString SelfJira;

	/**
	* The ID of the project.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	FString ID;

	/**
	* A brief description of the project.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	FString Description;

	/**
	* The URL of the issue type's avatar.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HTTP")
	FString IconUrl;

	/**
	* The name of the issue type.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	FString Name;

	/**
	* Whether this issue type is used to create subtasks.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	bool bIsSubtask;

	/**
	* The ID of the issue type's avatar.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	int64 AvatarID;

	/**
	* Unique ID for next-gen projects.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	FString EntityID;

	/**
	* Hierarchy level of the issue type.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	int32 HeirarchyLevel;

};


USTRUCT(BlueprintType)
struct JIRAPLUGIN_API FJiraProject
#if CPP
	: FJsonSerializable
#endif
{
	GENERATED_BODY()

#if CPP
public:
	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("expand", Expand);
		JSON_SERIALIZE("self", SelfJira);
		JSON_SERIALIZE("id", ID);
		JSON_SERIALIZE("key", Key);
		JSON_SERIALIZE("description", Description);
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("issueTypes", IssueTypes, FJiraIssueTypeDetails);
	END_JSON_SERIALIZER
#endif
public:

	/**
	* Expand options that include additional project details in the response.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	FString Expand;

	/**
	* The URL of the project details.
	* Format: uri
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HTTP", meta = (DisplayName = "Self"))
	FString SelfJira;

	/**
	* The ID of the project.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	FString ID;

	/**
	* The key of the project.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	FString Key;

	/**
	* A brief description of the project.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	FString Description;

	/**
	* The list of items.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	TArray<FJiraIssueTypeDetails> IssueTypes;

};

USTRUCT(BlueprintType)
struct JIRAPLUGIN_API FJiraPageBeanProject
#if CPP
	: FJsonSerializable
#endif
{
	GENERATED_BODY()

#if CPP
public:
	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("self", Self);
		JSON_SERIALIZE("nextPage", NextPage);
		JSON_SERIALIZE("maxResults", MaxResults);
		JSON_SERIALIZE("notHere", NotHere);
		JSON_SERIALIZE("startAt", StartAt);
		JSON_SERIALIZE("total", Total);
		JSON_SERIALIZE("isLast", bIsLast);
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("values", Values, FJiraProject);
	END_JSON_SERIALIZER
#endif
public:

	bool const HasSelf() {
		return Self.IsEmpty();
	}

	bool const HasNextPage() {
		return NextPage.IsEmpty();
	}

	bool const HasMaxResults() {
		return MaxResults != INT32_DEFAULT;
	}

	bool const HasStartAt() {
		return StartAt != INT64_DEFAULT;
	}

	bool const HasTotal() {
		return Total != INT64_DEFAULT;
	}

	bool const bIsLastPage() {
		return bIsLast;
	}

	bool const HasValues() {
		return Values.Num() > 0;
	}
	

	int32 NotHere = INT32_DEFAULT;

private:

	/**
	* The URL of the page.
	* Format: uri
	*/
	FString Self;

	/**
	* If there is another page of results, the URL of the next page.
	* Format: uri
	*/
	FString NextPage;

	/**
	* The maximum number of items that could be returned.
	* Format: int32
	*/
	int32 MaxResults;

	/**
	* The index of the first item returned.
	* Format: int64
	*/
	int64 StartAt;

	/**
	* The number of items returned.
	* Format: int64
	*/
	int64 Total;

	/**
	* Whether this is the last page.
	*/
	bool bIsLast;

	/**
	* The list of items.
	*/
	TArray<FJiraProject> Values;
};

