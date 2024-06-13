// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun/OVAOE_LightningStrike.h"

#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"

AOVAOE_LightningStrike::AOVAOE_LightningStrike()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Asset/Decal/Decal_AOE_Inst.Decal_AOE_Inst'"));
	if (DecalMaterialRef.Object)
	{
		DecalMaterial = DecalMaterialRef.Object;
	}
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	SetRootComponent(DecalComponent);
	DecalComponent->SetMaterial(0, DecalMaterial);
	DecalComponent->SetWorldScale3D(FVector(0.9f, 0.9f, 0.9f));
}



void AOVAOE_LightningStrike::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		Trigger();
		DecalComponent->SetVisibility(false); //삭제는 안하는데 삭제를 해야한다!!
	}, 1.0f, false);
}

void AOVAOE_LightningStrike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AOVAOE_LightningStrike::Trigger()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectArray;
	ObjectArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToNotTargeting;
	//ActorsToNotTargeting.Add(GetOwner());
	TArray<AActor*> HitResultActor;
	bool bResult =  UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 250,ObjectArray,nullptr, ActorsToNotTargeting, HitResultActor);
	DrawDebugSphere(GetWorld(), GetActorLocation(), 250, 24, FColor::Green, false, 2.0f);
	if(bResult)
	{
		for (AActor* Actor : HitResultActor)
		{
			OnOverlapActor.Broadcast(Actor);
		}
	}
}