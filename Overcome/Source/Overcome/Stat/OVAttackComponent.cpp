// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/OVAttackComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Character/OVCharacterPlayer.h"
#include "Interface/OVDamagableInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISense_Damage.h"

class AAIController;

UOVAttackComponent::UOVAttackComponent()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitRef(
TEXT("/Script/Niagara.NiagaraSystem'/Game/Vefects/Blood_VFX/VFX/Performance_Versions/Bullet_Hits/One_Shot/OS_NS_Bullet_Hit_Medium_Blue.OS_NS_Bullet_Hit_Medium_Blue'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}
}


void UOVAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UOVAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UOVAttackComponent::FireBullet(FVector Start, FVector End, FDamageInfo DamageInfo)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectArray;
	ObjectArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToNotTargeting;
	ActorsToNotTargeting.Add(GetOwner());
	FHitResult HitResult;
	// bool bResult = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),Start, End, 20, ObjectArray, false,ActorsToNotTargeting
	// ,EDrawDebugTrace::ForDuration, HitResult,  true,
	// 		FLinearColor::Red, FLinearColor::Green, 1.f);

	bool bResult = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1);
	if(bResult)
	{
		IOVDamagableInterface* DamagableInterface = Cast<IOVDamagableInterface>(HitResult.GetActor());
		if(DamagableInterface)
		{
			//
			DamagableInterface->TakeDamage(DamageInfo); //반환값 bool
			UAISense_Damage::ReportDamageEvent(
					GetWorld(),
					HitResult.GetActor(),        // Damaged Actor
					GetOwner(),                  // Instigator
					DamageInfo.Amount,     // Amount of Damage
					GetOwner()->GetActorLocation(),       // Event Location
					HitResult.ImpactPoint,       // Hit Location (same as event location for simplicity)
					FName(TEXT("DamageEvent"))   // Tag (optional)
				);
			OnAttackEnded.Broadcast();
		}
		//DrawDebugPoint(GetWorld(), HitResult.Location, 20, FColor::Red, true);
	}
	if(GetOwner()->IsA<AOVCharacterPlayer>())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EmitterHit, HitResult.Location, FRotator::ZeroRotator);
		//UE_LOG(LogTemp, Warning, TEXT("Character"));
	}
}

void UOVAttackComponent::AttackSlash(float Radius, float Length, FDamageInfo DamageInfo)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectArray;
	ObjectArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToNotTargeting;
	ActorsToNotTargeting.Add(GetOwner());
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = GetOwner()->GetActorForwardVector() * Length + Start;
	FHitResult HitResult;
	bool bResult = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),Start, End, Radius, ObjectArray, false,ActorsToNotTargeting
	,EDrawDebugTrace::ForDuration, HitResult,  true,
			FLinearColor::Red, FLinearColor::Green, 1.f);
	if(bResult)
	{
		IOVDamagableInterface* DamagableInterface = Cast<IOVDamagableInterface>(HitResult.GetActor());
		if(DamagableInterface)
		{
			DamagableInterface->TakeDamage(DamageInfo); //반환값 bool
		}
	}
}

