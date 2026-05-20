// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

class UItemDefinition;

UENUM()
enum class EItemType : uint8
{
	Tool UMETA(DisplayName = "Tool"),
	Consumable UMETA(DisplayName = "Consumable")
};

USTRUCT()
struct FItemText
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	// The ID name of this item for referencing in a table row.
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	// The type of the item.
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	// Text struct including the item name and description.
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemText ItemText;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	TSoftObjectPtr<UItemDefinition>ItemBase;
};