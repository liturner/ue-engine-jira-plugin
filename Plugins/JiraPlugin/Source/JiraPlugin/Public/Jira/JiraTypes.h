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
struct JIRAPLUGIN_API FJiraProject
{
	GENERATED_BODY()

public:

	/// <summary>
	/// Expand options that include additional project details in the response.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Expand;

	/// <summary>
	/// The URL of the project details.
	/// Format: uri
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Self"))
	FString SelfJira;

};

struct JIRAPLUGIN_API FJiraPageBeanProject : FJsonSerializable
{
public:
	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("self", Self);
		JSON_SERIALIZE("nextPage", NextPage);
		JSON_SERIALIZE("maxResults", MaxResults);
		JSON_SERIALIZE("notHere", NotHere);
		JSON_SERIALIZE("startAt", StartAt);
		JSON_SERIALIZE("total", Total);
		JSON_SERIALIZE("isLast", bIsLast);
		//JSON_SERIALIZE_ARRAY_SERIALIZABLE("values", Values, FJiraProject);
	END_JSON_SERIALIZER

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

	/// <summary>
	/// The URL of the page.
	/// Format: uri
	/// </summary>
	FString Self;

	/// <summary>
	/// If there is another page of results, the URL of the next page.
	/// Format: uri
	/// </summary>
	FString NextPage;

	/// <summary>
	/// The maximum number of items that could be returned.
	/// Format: int32
	/// </summary>
	int32 MaxResults;

	/// <summary>
	/// The index of the first item returned.
	/// Format: int64
	/// </summary>
	int64 StartAt;

	/// <summary>
	/// The number of items returned.
	/// Format: int64
	/// </summary>
	int64 Total;

	/// <summary>
	/// Whether this is the last page.
	/// </summary>
	bool bIsLast;

	/// <summary>
	/// The list of items.
	/// </summary>
	TArray<FJiraProject> Values;
};

