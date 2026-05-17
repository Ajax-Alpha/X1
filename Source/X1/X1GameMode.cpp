// Copyright Epic Games, Inc. All Rights Reserved.

#include "X1GameMode.h"

AX1GameMode::AX1GameMode()
{
	// stub
}
void AX1GameMode::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Hello World!"));
}