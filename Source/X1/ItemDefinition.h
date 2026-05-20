// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/ItemData.h"
#include "ItemDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class X1_API UItemDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// The ID name of this item for referencing in a table row.
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	// The type of this item.
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	// Text struct including the item name and description.
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemText ItemText;

	// The Static Mesh used to display this item in the world.
	UPROPERTY(EditAnywhere, Category = "Item Data")
	TSoftObjectPtr<UStaticMesh> WorldMesh;
};
