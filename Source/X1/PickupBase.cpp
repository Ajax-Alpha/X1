// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
#include "ItemDefinition.h"
#include "X1testCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

APickupBase::APickupBase()
{
    PrimaryActorTick.bCanEverTick = false; // 拾取物通常不需要 Tick 以节省性能

    PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    RootComponent = PickupMeshComponent; // 将 Mesh 设为根组件

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetupAttachment(RootComponent);
    SphereComponent->SetSphereRadius(32.f);
}

void APickupBase::BeginPlay()
{
    Super::BeginPlay();

    // 【关键修复】在此处仅绑定一次事件。
    // 不要放在 InitializePickup 中，否则每次重生都会重复绑定。
    if (SphereComponent)
    {
        SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnSphereBeginOverlap);
    }

    InitializePickup();
}

void APickupBase::InitializePickup()
{
    // 1. 检查数据表有效性
    if (!PickupDataTable.IsValid() || PickupItemID.IsNone())
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] 初始化失败: 数据表未分配或 ID 为空。"), *GetName());
        return;
    }

    UDataTable* Table = PickupDataTable.LoadSynchronous();
    if (!Table) return;

    // 2. 查找数据行
    const FItemData* ItemDataRow = Table->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());
    if (!ItemDataRow)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] 找不到 ID 为 '%s' 的数据行。"), *GetName(), *PickupItemID.ToString());
        return;
    }

    // 3. 【核心修复】使用 LoadSynchronous() 确保发射器等独立资源被强制加载。
    // 这解决了 .Get() 在资源未预加载时返回 null 导致重现失败的问题。
    UItemDefinition* TempItemDefinition = ItemDataRow->ItemBase.LoadSynchronous();
    if (!TempItemDefinition)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] 无法加载 ItemDefinition。"), *GetName());
        return;
    }

    // 4. 更新逻辑数据引用
    ReferenceItem = TempItemDefinition->CreateItemCopy();

    // 5. 更新并恢复显示状态
    UStaticMesh* MeshToUse = TempItemDefinition->WorldMesh.LoadSynchronous();
    if (PickupMeshComponent && MeshToUse)
    {
        PickupMeshComponent->SetStaticMesh(MeshToUse);
        PickupMeshComponent->SetVisibility(true);
        PickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

    // 6. 重新激活触发器碰撞
    if (SphereComponent)
    {
        SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }

    UE_LOG(LogTemp, Log, TEXT("%s Item respawned and ready."), *GetName());
}

void APickupBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AX1testCharacter* Character = Cast<AX1testCharacter>(OtherActor);

    if (Character != nullptr)
    {
        // 1. 执行拾取逻辑（给玩家物品）
        Character->GiveItem(ReferenceItem);

        // 2. 隐藏并禁用碰撞，防止在 4 秒重现期内再次触发
        if (PickupMeshComponent)
        {
            PickupMeshComponent->SetVisibility(false);
            PickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

        if (SphereComponent)
        {
            SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

        // 3. 启动重生计时器
        if (bShouldRespawn)
        {
            // 按要求保留 4.0f 初始延迟参数
            GetWorldTimerManager().SetTimer(
                RespawnTimerHandle,
                this,
                &APickupBase::InitializePickup,
                RespawnTime,
                false,
                4.0f
            );
        }
    }
}

void APickupBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void APickupBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickupBase, PickupItemID))
    {
        // 编辑器下实时预览更新
        InitializePickup();
    }
}
#endif