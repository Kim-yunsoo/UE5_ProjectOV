// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun/OVAOE_LightningStrike.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "NiagaraComponent.h"

class UNiagaraSystem;

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

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> EmitterRef(
TEXT("/Script/Niagara.NiagaraSystem'/Game/EnergyBeam/NS/NS_IonLaser.NS_IonLaser'"));

	if (EmitterRef.Succeeded())
	{
		Emitter = EmitterRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitRef(
TEXT("/Script/Niagara.NiagaraSystem'/Game/EnergyBeam/NS/NS_Hit_Constant_IonLaser.NS_Hit_Constant_IonLaser'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}
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
	//전기 이펙트 여기서!
	FVector Location = GetActorLocation();
	Location.Z += 1000;
	FTimerHandle TimerHandle;
	FVector LocationHit = GetActorLocation();
	LocationHit.Z -= 50;
	// 파티클 시스템을 생성하고 변수에 저장합니다.
	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Emitter, Location, FRotator(-90, 0, 0));
	UNiagaraComponent* NiagaraComponentHit = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EmitterHit, LocationHit, FRotator(0, 0, 0));
	// 1초 후에 파티클 시스템을 제거하는 타이머를 설정합니다.
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [NiagaraComponent, NiagaraComponentHit]() {
		if (NiagaraComponent)
		{
			NiagaraComponent->DestroyComponent();
			NiagaraComponentHit->DestroyComponent();
		}
	}, 1.0f, false);
	
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