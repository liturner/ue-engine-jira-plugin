// Copyright TD Technologies. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FJiraPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
