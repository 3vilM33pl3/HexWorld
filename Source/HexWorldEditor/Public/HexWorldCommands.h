// Copyright (c) 2022, Robot Motel Limited, All Rights Reserved.

#pragma once

class FHexWorldCommands : public TCommands<FHexWorldCommands>
{
public:
	FHexWorldCommands();
	virtual void RegisterCommands() override;

	static TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetCommands();

	TSharedPtr<FUICommandInfo> HexWorldRetrieveMap;
	TSharedPtr<FUICommandInfo> HexWorldEditHex;

protected:
	TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> Commands;
};
