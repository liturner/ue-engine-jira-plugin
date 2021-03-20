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
		return false;
	}

	// "luke.turner@tdtek.de:7YVZF0f5pprkBvQyU7pWD190"
	HttpRequestRef->AppendToHeader("Authorization", "Basic " + FBase64::Encode(UserEmail + ":" + ApiToken));
	HttpRequestRef->AppendToHeader("Accept", "application/json");
	HttpRequestRef->SetURL(ServerUrl + HttpRequestRef->GetURL());
	return HttpRequestRef->ProcessRequest();
}
