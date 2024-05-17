// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/OVItemBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/OVCharacterItemInterface.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AOVItemBase::AOVItemBase()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);
	
	Trigger->SetBoxExtent(FVector(50.f, 50.0f, 50.f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AOVItemBase::OnOverlapBegin);
	Trigger->SetCollisionProfileName(TEXT("Item"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Item/SM_Item.SM_Item'"));
	if(MeshRef.Object)
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -60.f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	
}

void AOVItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if(nullptr == Item)
	{
		Destroy();
		return;
	}
	
	IOVCharacterItemInterface* OverlappingPawn = Cast<IOVCharacterItemInterface>(OtherActor);
	if(OverlappedComp)
	{
		OverlappingPawn->TakeItem(Item);
	}
	
	//SetActorEnableCollision(false);

	//3초 후 삭제
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		// this->Destroy();
		// Mesh->SetHiddenInGame(true);
	
	}, 3.0f, false);
}



