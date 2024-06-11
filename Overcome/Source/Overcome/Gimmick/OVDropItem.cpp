// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/OVDropItem.h"
#include "Components/BoxComponent.h"
#include "Game/OVGameMode.h"
#include "Game/OVGameState.h"
#include "Item/OVItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Object/OVPickup.h"

AOVDropItem::AOVDropItem()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Cylinder_F3406963.Cylinder_F3406963'"));
	if (MeshRef.Object)
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Mesh);
	Battery = 5;
}

void AOVDropItem::BeginPlay()
{
	Super::BeginPlay();
	Trigger->SetBoxExtent(FVector(50.f, 50.0f, 50.f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AOVDropItem::OnTriggerEnter);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AOVDropItem::OnTriggerExit);
}

void AOVDropItem::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOVPickup* Pickup = Cast<AOVPickup>(OtherActor);
	if (Pickup && OtherComp == Pickup->GetMeshComponent() && Pickup->GetItemData()->ItemType == EItemType::Battery)
	{
		if (AOVGameMode* GameMode = Cast<AOVGameMode>(GetWorld()->GetAuthGameMode()))
		{
			Battery -=1;
			GameMode->SetBatteryCount(Battery);
		}
	}
}

void AOVDropItem::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	
}

void AOVDropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

