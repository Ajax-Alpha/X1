// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquippableToolBase.generated.h"

class AX1testCharacter;
class UInputAction;
class UInputMappingContext;

UCLASS()
class X1_API AEquippableToolBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquippableToolBase();

	// First Person animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimBlueprint> FirstPersonToolAnim;

	// Third Person animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimBlueprint> ThirdPersonToolAnim;

	// The input mapping context associated with this tool
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> ToolMappingContext;

	// Tool Skeletal Mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> ToolMeshComponent;

	// The character holding this tool
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AX1testCharacter> OwningCharacter;

	// First Person animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UseCooldown;

	// Use the tool
	UFUNCTION()
	virtual void Use();

	// Binds the Use function to the owning character
	UFUNCTION()
	virtual void BindInputAction(const UInputAction* ActionToBind);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals")
	USkeletalMeshComponent* Mesh1P;

	// 第三人称模型（其他人看到的，以及用来投射阴影的武器）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals")
	USkeletalMeshComponent* Mesh3P;

	// 动画资源引用（从你的代码结构来看，这些应该是 DataAsset 或类成员）
	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> FirstPersonAnimClass;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> ThirdPersonAnimClass;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
