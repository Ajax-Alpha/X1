// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
#include "ItemDefinition.h"
#include "X1testCharacter.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    check(PickupMeshComponent != nullptr);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	check(SphereComponent != nullptr);

	SphereComponent->SetupAttachment(PickupMeshComponent);
	
	SphereComponent->SetSphereRadius(32.f);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePickup();
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupBase::InitializePickup()


{
    // 1. 基础检查：确保数据表和ID有效
    if (!PickupDataTable || PickupItemID.IsNone())
    {
        UE_LOG(LogTemp, Warning, TEXT("InitializePickup Failed: DataTable is null or ID is None."));
        return;
    }

    // 2. 查找数据行
    const FItemData* ItemDataRow = PickupDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());

    // 3. 检查行是否找到（这是最常见的崩溃点）
    if (!ItemDataRow)
    {
        UE_LOG(LogTemp, Error, TEXT("InitializePickup Failed: Row '%s' not found in DataTable!"), *PickupItemID.ToString());
        return;
    }

    // 4. 检查 ItemBase 软引用是否有效（数据表里有没有填）
    if (!ItemDataRow->ItemBase.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("InitializePickup Failed: ItemBase is null in Row '%s'. Check your DataTable!"), *PickupItemID.ToString());
        return;
    }

    // 5. 获取 ItemDefinition 对象指针
    UItemDefinition* TempItemDefinition = ItemDataRow->ItemBase.Get();

    if (!TempItemDefinition)
    {
        // 如果 Get() 返回空，说明资源还没加载，尝试同步加载
        UE_LOG(LogTemp, Warning, TEXT("ItemDefinition not loaded, loading synchronously for %s"), *PickupItemID.ToString());
        TempItemDefinition = ItemDataRow->ItemBase.LoadSynchronous();
    }

    if (!TempItemDefinition)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load ItemDefinition for %s"), *PickupItemID.ToString());
        return;
    }

    // --- 此时数据都安全了，开始赋值 ---

    // 6. 初始化 ReferenceItem (如果它是用于存储数据的副本)
    if (!ReferenceItem)
    {
        ReferenceItem = NewObject<UItemDefinition>(this, UItemDefinition::StaticClass());
    }

    // 复制数据
    ReferenceItem->ID = ItemDataRow->ID;
    ReferenceItem->ItemType = ItemDataRow->ItemType;
    ReferenceItem->ItemText = ItemDataRow->ItemText;

    // 注意：这里直接从 TempItemDefinition 复制 Mesh 引用
    ReferenceItem->WorldMesh = TempItemDefinition->WorldMesh;

    // 7. 处理 Mesh 显示
    UStaticMesh* MeshToUse = nullptr;

    // 检查 WorldMesh 软引用是否有效
    if (TempItemDefinition->WorldMesh.IsValid())
    {
        MeshToUse = TempItemDefinition->WorldMesh.Get();
    }
    else
    {
        // 如果 Mesh 没加载，同步加载它
        UE_LOG(LogTemp, Warning, TEXT("WorldMesh not loaded, loading synchronously."));
        MeshToUse = TempItemDefinition->WorldMesh.LoadSynchronous();
    }

    // 8. 最终应用到组件
    if (PickupMeshComponent && MeshToUse)
    {
        PickupMeshComponent->SetStaticMesh(MeshToUse);
        PickupMeshComponent->SetVisibility(true);
        PickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Mesh is null or Component is null!"));
        // 即使Mesh为空，也要确保组件状态正确，或者隐藏Actor
        if (PickupMeshComponent) PickupMeshComponent->SetVisibility(false);
    }

    // 9. 激活碰撞
    if (SphereComponent)
    {
        SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnSphereBeginOverlap);
    }
}

void APickupBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Attempting a pickup collision"));
    ACharacter* Character = Cast<ACharacter>(OtherActor);

    if (Character != nullptr)
    {
        SphereComponent->OnComponentBeginOverlap.RemoveAll(this);

        PickupMeshComponent->SetVisibility(false);
        PickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    if (bShouldRespawn)
    {
        GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APickupBase::InitializePickup, RespawnTime, false, 0);
    }
}

void APickupBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    // Handle parent class property changes
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // If a property was changed, get the name of the changed property. Otherwise use none.
    const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    // Verify that the changed property exists in this class and that the PickupDataTable is valid.
    if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickupBase, PickupItemID) && PickupDataTable)
    {
        // Retrieve the associated ItemData for this pickup.
        if (const FItemData* ItemDataRow = PickupDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString()))
        {
            UItemDefinition* TempItemDefinition = ItemDataRow->ItemBase.Get();

            // Set the pickup's mesh to the associated item's mesh
            PickupMeshComponent->SetStaticMesh(TempItemDefinition->WorldMesh.Get());

            // Set the sphere's collision radius
            SphereComponent->SetSphereRadius(32.f);
        }
    }
}