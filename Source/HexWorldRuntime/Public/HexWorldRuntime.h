// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

class FHexWorldRuntime : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
