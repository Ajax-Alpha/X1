// Fill out your copyright notice in the Description page of Project Settings.


#include "EquippableToolBase.h"
#include "X1testCharacter.h"

// Sets default values
AEquippableToolBase::AEquippableToolBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // 武器通常不需要每帧执行逻辑

	// 创建一个根组件
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	// 初始化 1P 模型
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh1P"));
	Mesh1P->SetupAttachment(RootComponent);
	Mesh1P->SetOnlyOwnerSee(true);        // 只有持有者可见
	Mesh1P->CastShadow = false;          // 1P不投射阴影

	// 初始化 3P 模型
	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh3P"));
	Mesh3P->SetupAttachment(RootComponent);
	Mesh3P->SetOwnerNoSee(true);         // 持有者不可见
	Mesh3P->CastShadow = true;           // 3P投射阴影
}

// Called when the game starts or when spawned
void AEquippableToolBase::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Tool BeginPlay"));
}

void AEquippableToolBase::Use()
{
}

void AEquippableToolBase::BindInputAction(const UInputAction* ActionToBind)
{
}
// Called every frame
void AEquippableToolBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

