// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/OVAttackComponent.h"

#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/OVCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/OVDamagableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

void UOVAttackComponent::AttackAOESlash(float Radius, FDamageInfo DamageInfo)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectArray;
	ObjectArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToNotTargeting;
	ActorsToNotTargeting.Add(GetOwner());
	TArray<AActor*> HitResultActor;
	bool bResult =  UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetOwner()->GetActorLocation(), Radius,ObjectArray,nullptr, ActorsToNotTargeting, HitResultActor);
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), Radius, 24, FColor::Green, false, 2.0f);
	if(bResult)
	{
		for (AActor* Actor : HitResultActor)
		{
			IOVDamagableInterface* DamagableInterface = Cast<IOVDamagableInterface>(Actor);
			if (DamagableInterface)
			{
				UE_LOG(LogTemp,Warning, TEXT("AOESlash"));
				DamagableInterface->TakeDamage(DamageInfo);
			}
		}
	}
}

void UOVAttackComponent::AttackAOEStrike(FDamageInfo DamageInfo)
{
	
}

void UOVAttackComponent::JumpTarget(AActor* AttackTarget)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(OwnerCharacter && AttackTarget)
	{
		FVector Location = CalculateFutureActorLocation(AttackTarget, 1.0f);
		FVector OutLaunchVelocity;
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Location.ToString())
		double BossTargetDistance;
		BossTargetDistance = AttackTarget->GetDistanceTo(GetOwner());
		BossTargetDistance = UKismetMathLibrary::NormalizeToRange(BossTargetDistance, 400, 800);
		BossTargetDistance = UKismetMathLibrary::FClamp(BossTargetDistance, 0.0, 1.0);
		BossTargetDistance = UKismetMathLibrary::Lerp(0.5, 0.94, BossTargetDistance);
		Location.Z += 50;
		//UE_LOG(LogTemp, Warning, TEXT("%f"), BossTargetDistance)
		//UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), OutLaunchVelocity, GetOwner()->GetActorLocation(),AttackTarget->GetActorLocation(), 0.0, 0.5 );
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), OutLaunchVelocity, GetOwner()->GetActorLocation(),Location, 0.0, BossTargetDistance );
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *OutLaunchVelocity.ToString());

		//FVector TestLocation {0,0,500};
		OwnerCharacter->LaunchCharacter(OutLaunchVelocity, true, true);
		//OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
		OwnerCharacter->LandedDelegate.AddDynamic(this, &UOVAttackComponent::OnCharacterLanded);
	}
}

FVector UOVAttackComponent::CalculateFutureActorLocation(AActor* AttackTarget, float Timer)
{
	FVector Result;
	Result = (AttackTarget->GetVelocity() * FVector(1, 1, 0))  * Timer + AttackTarget->GetActorLocation();
	return Result;
}

void UOVAttackComponent::OnCharacterLanded(const FHitResult& Hit)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(OwnerCharacter)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OnLanded"));
		OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
		OwnerCharacter->LandedDelegate.RemoveDynamic(this, &UOVAttackComponent::OnCharacterLanded);
	}
}

