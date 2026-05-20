// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "X1testCharacter.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class UItemDefinition;

UCLASS(BlueprintType, Blueprintable)
class X1_API APickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Table")
	FName PickupItemID;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Table")
	TSoftObjectPtr<UDataTable> PickupDataTable;

	// Data asset associated with this item.
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Reference Item")
	TSoftObjectPtr<UItemDefinition> ReferenceItem;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializePickup();



};
