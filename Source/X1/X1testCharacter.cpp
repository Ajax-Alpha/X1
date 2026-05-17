// Fill out your copyright notice in the Description page of Project Settings.


#include "X1testCharacter.h"

// Sets default values
AX1testCharacter::AX1testCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AX1testCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AX1testCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AX1testCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

