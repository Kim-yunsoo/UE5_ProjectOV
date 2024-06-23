// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/OVPickup.h"
#include "Component/OVInventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Item/OVItemBase.h"

// Sets default values
AOVPickup::AOVPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(Trigger);
	PickupMesh->SetMobility(EComponentMobility::Movable);
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);
	//PickupMesh->SetCollisionProfileName(TEXT("NoCollision"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(PickupMesh);
	Trigger->SetBoxExtent(FVector(45.f, 45.f, 45.f));
    Trigger->SetMobility(EComponentMobility::Movable);
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Script/Engine.DataTable'/Game/ItemData/DT_ItemData.DT_ItemData'"));
	if (DataTableFinder.Succeeded())
	{
		ItemDataTable = DataTableFinder.Object;
		ItemQuantity = 1;
		PickupMesh->BodyInstance.bOverrideMass = true;
		PickupMesh->BodyInstance.SetMassOverride(200);
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> HPPotionMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Asset/Outline/M_Outline_Hp.M_Outline_Hp'"));
	if (HPPotionMaterialRef.Succeeded())
	{
		HPPotionMaterial = HPPotionMaterialRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MPPotionMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Asset/Outline/M_Outline_Mp.M_Outline_Mp'"));
	if (MPPotionMaterialRef.Succeeded())
	{
		MPPotionMaterial = MPPotionMaterialRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> AttackPotionMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Asset/Outline/M_Outline_Attack.M_Outline_Attack'"));
	if (AttackPotionMaterialRef.Succeeded())
	{
		AttackPotionMaterial = AttackPotionMaterialRef.Object;
	}
}

// Called when the game starts or when spawned
void AOVPickup::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AOVPickup::OnTriggerEnter);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AOVPickup::OnTriggerExit);
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

void AOVPickup::InitialStart()
{
	if(DesiredItemID == "HPPotion")
		PickupMesh->SetOverlayMaterial(HPPotionMaterial);
	else if(DesiredItemID == "MPPotion")
		PickupMesh->SetOverlayMaterial(MPPotionMaterial);
	else if(DesiredItemID == "AttackPotion")
		PickupMesh->SetOverlayMaterial(AttackPotionMaterial);
	InitializePickup(UOVItemBase::StaticClass(),ItemQuantity);
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

void AOVPickup::TakePickup(AOVCharacterPlayer* Taker)
{
	if(!IsPendingKillPending()) // 액터가 현재 파괴중이 아니라면 ?? 
	{
		if(ItemReference)
		{
			if (UOVInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);
				Taker->PlayPickupMontage();
				switch (AddResult.OperationResult)
				{
				case EItemAddResult::IAR_NoItemAdded:
					break;
				case EItemAddResult::IAR_PartialAmountItemAdd:
					UpdateInteractableData();
					Taker->UpdateInteractionWidget();
					break;
				case EItemAddResult::IAR_AllItemAdded:
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
					{
						Destroy(); //완전히 인벤토리에 추가 됨을 나타낸다. 삭제가 일어난다. 
					}, 0.8f, false);
					break;
				}
				UE_LOG(LogTemp,Warning,TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("Player inventory component is null!"));
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Pickup internal item reference was somehow null!"));
		}
	}
}

void AOVPickup::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOVCharacterPlayer* MyPlayer = Cast<AOVCharacterPlayer>(OtherActor);
	if(MyPlayer)
	{
		MyPlayer->PerformInteractionCheck(this);
	}
}

void AOVPickup::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	AOVCharacterPlayer* MyPlayer = Cast<AOVCharacterPlayer>(OtherActor);
	if(MyPlayer)
	{
		MyPlayer->NoInteractableFound();
	}
}



// void AOVPickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) //에디터에서 메쉬를 바꿀 때 바로 로드되도록 해줌
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
//
// 	const FName ChangePropertyName = PropertyChangedEvent.Property? PropertyChangedEvent.Property->GetFName():NAME_None;
//
// 	if(ChangePropertyName == GET_MEMBER_NAME_CHECKED(AOVPickup,DesiredItemID))
// 	{
// 		if(ItemDataTable)
// 		{
// 			if(const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID,DesiredItemID.ToString()))
// 			{
// 				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
// 			}
// 			
// 		}
// 	}
// }
