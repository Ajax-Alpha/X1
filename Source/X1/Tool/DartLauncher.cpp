// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool/DartLauncher.h"
#include "FirstPersonProjectile.h"  
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h" 
#include "X1/X1testCharacter.h"

void ADartLauncher::Use()
{
	// 增加安全检查
	if (!OwningCharacter || !ProjectileClass) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Using the dart launcher!"));

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FVector TargetPosition = OwningCharacter->GetCameraTargetLocation();

		// 【修改】从 Mesh1P 获取枪口位置，因为 1P 视角下子弹应从可见的枪管飞出
		// 确保你在手枪模型的骨骼中添加了名为 "Muzzle" 的 Socket
		FVector SocketLocation = Mesh1P->GetSocketLocation(FName("Muzzle"));
		FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation, TargetPosition);

		// 稍微往前偏移一点，防止子弹生成时立刻碰撞到枪管
		FVector SpawnLocation = SocketLocation + UKismetMathLibrary::GetForwardVector(SpawnRotation) * 10.0f;

		FActorSpawnParameters ActorSpawnParams;
		// 这里的 Owner 设为 OwningCharacter
		ActorSpawnParams.Owner = OwningCharacter;
		ActorSpawnParams.Instigator = OwningCharacter->GetInstigator();
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		World->SpawnActor<AFirstPersonProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}
}

	


void ADartLauncher::BindInputAction(const UInputAction* InputToBind)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(OwningCharacter->GetController()))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{

			EnhancedInputComponent->BindAction(InputToBind, ETriggerEvent::Triggered, this, &ADartLauncher::Use);
		}
	}
}