#pragma once
#include "Modules/ModuleInterface.h"

class HEXWORLDEDITOR_API FHexWorldEditor : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

