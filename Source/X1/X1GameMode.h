// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "X1GameMode.generated.h"

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class AX1GameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void StartPlay() override;
public:
	AX1GameMode();
};



