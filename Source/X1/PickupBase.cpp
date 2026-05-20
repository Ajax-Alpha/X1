// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
#include "ItemDefinition.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
    
    if (PickupDataTable && !PickupItemID.IsNone())
    {
        const FItemData* ItemDataRow = PickupDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());

        
        if (ItemDataRow)
        {
          
            ReferenceItem = NewObject<UItemDefinition>(this, UItemDefinition::StaticClass());

           
            ReferenceItem->ID = ItemDataRow->ID;
            ReferenceItem->ItemType = ItemDataRow->ItemType;
            ReferenceItem->ItemText = ItemDataRow->ItemText;

         
            if (ItemDataRow->ItemBase.Get())
            {
                ReferenceItem->WorldMesh = ItemDataRow->ItemBase.Get()->WorldMesh;
            }
        }
    }
}