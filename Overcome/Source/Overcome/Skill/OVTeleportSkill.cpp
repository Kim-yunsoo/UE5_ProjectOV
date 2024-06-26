// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/OVTeleportSkill.h"
#include "Character/OVCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


UOVTeleportSkill::UOVTeleportSkill()
{
	PrimaryComponentTick.bCanEverTick = true;
	TeleportOffset = 600.0f;
	TeleportCooltime = 5.f;
	CooldownRemaining = 5.f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EmitterRef(
TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/ICE/combat/P_Shout_Charge_Ice_02.P_Shout_Charge_Ice_02'"));

	if (EmitterRef.Succeeded())
	{
		Emitter = EmitterRef.Object;
	}
}

void UOVTeleportSkill::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Owner)
	{
		return;
	}
	float CooldownPercent = 0.f;
	
	if (Owner->bIsActiveTeleportSkill) 
	{
		OnTeleportTime.Broadcast(CooldownPercent);
		return;
	}
	
	if (CooldownRemaining > 0)
	{
		CooldownRemaining -= DeltaTime;
		if (CooldownRemaining < 0)
		{
			CooldownRemaining = 0;
		}
		// 위젯 업데이트
		CooldownPercent = CooldownRemaining / TeleportCooltime;
		
		OnTeleportTime.Broadcast(CooldownPercent);
	}
}

void UOVTeleportSkill::SkillAction()
{
	UE_LOG(LogSkillCharacter, Log, TEXT("StartTeleportSkill"));
	Super::SkillAction();
	
	UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(
		   GetWorld(), Emitter, GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() *600, FRotator(0, 90, 0));

	ParticleSystemComponent->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));


	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectArray;
	ObjectArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<AActor*> ActorsToNotTargeting;
	ActorsToNotTargeting.Add(GetOwner());
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = GetOwner()->GetActorForwardVector() * TeleportOffset + Start;
	FHitResult HitResult;
	bool bResult = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),Start, End, 40, ObjectArray, false,ActorsToNotTargeting
	,EDrawDebugTrace::None, HitResult,  true,
			FLinearColor::Red, FLinearColor::Green, 1.f);
	if(bResult)
	{
		GetOwner()->TeleportTo(HitResult.Location, GetOwner()->GetActorRotation(), false, true);
	}
	else
	{
		GetOwner()->TeleportTo(End, GetOwner()->GetActorRotation(), false, true);
	}
	
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		{
			Owner->bIsActiveTeleportSkill = true;
			CooldownRemaining = 5;
			UE_LOG(LogSkillCharacter, Log, TEXT("EndTeleportSkill"));
		}
	), TeleportCooltime, false, -1.0f);
}