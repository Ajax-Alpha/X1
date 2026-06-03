// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "X1testCharacter.generated.h"

class AEquippableToolBase;
class UItemDefinition;
class UInputComponent;
class UAnimBlueprint;
class UInputMappingContext;
class UInputAction;
class UInventoryComponent;
class UEquippableToolDefinition;

UCLASS()
class X1_API AX1testCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AX1testCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> FirstPersonContext;

	// Move Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	// Jump Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input)
	TObjectPtr<UInputAction> UseAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tools)
	TObjectPtr<AEquippableToolBase> EquippedTool;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	UPROPERTY(VisibleAnywhere,Category = Camera)
	UCameraComponent* FirstPersonCameraComponent;

	// Offset for the first-person camera
	UPROPERTY(EditAnywhere, Category = Camera)
	FVector FirstPersonCameraOffset = FVector(2.8f, 5.9f, 0.0f);

	UPROPERTY(EditAnywhere,Category = Camera)
	float FirstPersonFieldOfView = 70.0f;

	UPROPERTY(EditAnywhere, Category = Camera)
	float FirstPersonViewScale = 0.6f;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	USkeletalMeshComponent* FirstPersonMeshComponent;
	
	// First Person animations
	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimBlueprint* FirstPersonDefaultAnim;

	// Inventory Component
	UPROPERTY(VisibleAnywhere, Category = Inventory)
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UFUNCTION()
	bool IsToolAlreadyOwned(UEquippableToolDefinition* ToolDefinition);

	UFUNCTION()
	void AttachTool(UEquippableToolDefinition* ToolDefinition);

	UFUNCTION()
	void GiveItem(UItemDefinition* ItemDefinition);

	UFUNCTION()
	FVector GetCameraTargetLocation();
};
