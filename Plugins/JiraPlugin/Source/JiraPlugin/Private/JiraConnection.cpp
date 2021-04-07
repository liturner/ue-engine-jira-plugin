#include "JiraConnection.h"

#include "Misc/Base64.h"

AJiraConnection::AJiraConnection() 
{
	// We hide some categories from the User, so make sure the defaults are correct
	// LOD Category
	bEnableAutoLODGeneration = false;
	
	// Input Category
	AutoReceiveInput = EAutoReceiveInput::Type::Disabled;

	// Collision Category
	bGenerateOverlapEventsDuringLevelStreaming = false;
	UpdateOverlapsMethodDuringLevelStreaming = EActorUpdateOverlapsMethod::NeverUpdate;
	bRelevantForLevelBounds = false;

	// Actor Category
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SetCanBeDamaged(false);
}

bool AJiraConnection::CanAuthenticate(const AJiraConnection* JiraConnection)
{
	if (JiraConnection == NULL)
	{
		return false;
	}

	if (JiraConnection->UserEmail.IsEmpty() || JiraConnection->ApiToken.IsEmpty() || JiraConnection->ServerUrl.IsEmpty())
	{
		return false;
	}
	return true;
}

FHttpRequestRef AJiraConnection::CreateRequest()
{
	return FHttpModule::Get().CreateRequest();
}

bool AJiraConnection::ProcessRequest(FHttpRequestRef HttpRequestRef)
{
	if (!CanAuthenticate(this))
	{
		// We insert an "Abort Code" into the Request which can later be used to detect why the request was cancelled
		HttpRequestRef->AppendToHeader("AbortCode", "499");
		return false;
	}

	HttpRequestRef->AppendToHeader("Authorization", "Basic " + FBase64::Encode(UserEmail + ":" + ApiToken));
	HttpRequestRef->AppendToHeader("Accept", "application/json");
	HttpRequestRef->SetURL(ServerUrl + HttpRequestRef->GetURL());
	bool requestStarted = HttpRequestRef->ProcessRequest();

	if (!requestStarted)
	{
		HttpRequestRef->AppendToHeader("AbortCode", "599");
	}

	return requestStarted;
}
