// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/OVPickup.h"

#include "Item/OVItemBase.h"

// Sets default values
AOVPickup::AOVPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);
}

// Called when the game starts or when spawned
void AOVPickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UOVItemBase::StaticClass(),ItemQuantity);
	
}

void AOVPickup::InitializePickup(const TSubclassOf<UOVItemBase> BaseClass, const int32 InQuantity)
{
	if(ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		ItemReference = NewObject<UOVItemBase>(this,BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->ItemNumericData = ItemData->ItemNumericData;
		ItemReference->AssetData = ItemData->AssetData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();


	}
		
}

void AOVPickup::InitializeDrop(UOVItemBase* ItemDrop, const int32 InQuantity)
{
	ItemReference = ItemDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	PickupMesh->SetStaticMesh(ItemDrop->AssetData.Mesh);
	UpdateInteractableData();
}

void AOVPickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void AOVPickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void AOVPickup::Interact(AOVCharacterPlayer* PlayerCharacter)
{
	if(PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}
}

void AOVPickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.Description;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}

void AOVPickup::TakePickup(const AOVCharacterPlayer* Taker)
{
	if(!IsPendingKillPending()) // 액터가 현재 파괴중이 아니라면 ?? 
	{
		//if(UInventoryComponent* PlayerInventory = Taker->GetInventory())
		
	}
}

void AOVPickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) //에디터에서 메쉬를 바꿀 때 바로 로드되도록 해줌
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangePropertyName = PropertyChangedEvent.Property? PropertyChangedEvent.Property->GetFName():NAME_None;

	if(ChangePropertyName == GET_MEMBER_NAME_CHECKED(AOVPickup,DesiredItemID))
	{
		if(ItemDataTable)
		{
			if(const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID,DesiredItemID.ToString()))
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
			
		}
	}
}
