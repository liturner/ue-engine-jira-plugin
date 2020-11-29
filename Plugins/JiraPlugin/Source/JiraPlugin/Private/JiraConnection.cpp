#include "Jira/JiraConnection.h"

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
	if (JiraConnection->UserEmail.IsEmpty() || JiraConnection->ApiToken.IsEmpty() || JiraConnection->ServerUrl.IsEmpty())
	{
		return false;
	}
	return true;
}
