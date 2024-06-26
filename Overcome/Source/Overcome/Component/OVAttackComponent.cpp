// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/OVAttackComponent.h"

#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/OVCharacterPlayer.h"
#include "Character/AI/OVAIBoss.h"
#include "Character/AI/OVCharacterNonPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gimmick/OVDamageWidgetActor.h"
#include "Interface/OVDamagableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISense_Damage.h"
#include "Player/OVPlayerController.h"

class AAIController;

UOVAttackComponent::UOVAttackComponent()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitRef(
TEXT("/Script/Niagara.NiagaraSystem'/Game/SlashHitVFX/NS/NS_Hit_Stick.NS_Hit_Stick'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BossRef(
TEXT("/Script/Niagara.NiagaraSystem'/Game/SlashHitVFX/NS/NS_Hit_Halberd.NS_Hit_Halberd'"));

	if (BossRef.Succeeded())
	{
		BossHit = BossRef.Object;
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
	bool bResult = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1);
	if(bResult && HitResult.GetActor() != GetOwner())
	{
		IOVDamagableInterface* DamagableInterface = Cast<IOVDamagableInterface>(HitResult.GetActor());
		if(DamagableInterface)
		{
			DamagableInterface->TakeDamage(DamageInfo);
			FActorSpawnParameters SpawnParams;
			AOVDamageWidgetActor* DamageActor = GetWorld()->SpawnActor<AOVDamageWidgetActor>(AOVDamageWidgetActor::StaticClass() , HitResult.Location, FRotator::ZeroRotator, SpawnParams );
			if (DamageActor)
			{
				ACharacter *Character = Cast<ACharacter>(HitResult.GetActor());
				DamageActor->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
				FVector Loc = HitResult.Location;
				DamageActor->SetActorLocation(Loc);

				AOVDamageWidgetActor* Widget = Cast<AOVDamageWidgetActor>(DamageActor);
				if (Widget)
				{
					Widget->SetDamage(DamageInfo.Amount);
				}
			}
			UAISense_Damage::ReportDamageEvent(
					GetWorld(),
					HitResult.GetActor(),        
					GetOwner(),                
					DamageInfo.Amount,    
					GetOwner()->GetActorLocation(),     
					HitResult.ImpactPoint,       
					FName(TEXT("DamageEvent")) 
				);
			OnAttackEnded.Broadcast();
		}
		if(Cast<AOVCharacterNonPlayer>(HitResult.GetActor()))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EmitterHit, HitResult.Location, FRotator::ZeroRotator);
		}
		else if(Cast<AOVAIBoss>(HitResult.GetActor()))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BossHit, HitResult.Location, FRotator::ZeroRotator);
		}
	}
}

void UOVAttackComponent::AttackSlash(float Radius, float Length, FDamageInfo DamageInfo)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectArray;
	ObjectArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
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
			UE_LOG(LogTemp,Warning, TEXT("AttackSlash"));
			DamagableInterface->TakeDamage(DamageInfo); //반환값 bool
		}
	}
}

void UOVAttackComponent::AttackAOESlash(float Radius, FDamageInfo DamageInfo)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectArray;
	ObjectArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToNotTargeting;
	ActorsToNotTargeting.Add(GetOwner());
	TArray<AActor*> HitResultActor;
	bool bResult =  UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetOwner()->GetActorLocation(), Radius,ObjectArray,nullptr, ActorsToNotTargeting, HitResultActor);
	//DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), Radius, 24, FColor::Green, false, 2.0f);
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
	ACharacter* TargetCharacter = Cast<ACharacter>(AttackTarget);
	if(OwnerCharacter && AttackTarget)
	{
		FVector Location = CalculateFutureActorLocation(AttackTarget, 1.0f);
		FVector OutLaunchVelocity;
		double BossTargetDistance = AttackTarget->GetDistanceTo(GetOwner());
		BossTargetDistance = UKismetMathLibrary::NormalizeToRange(BossTargetDistance, 400, 800);
		BossTargetDistance = UKismetMathLibrary::FClamp(BossTargetDistance, 0.0, 1.0);
		BossTargetDistance = UKismetMathLibrary::Lerp(0.5, 0.94, BossTargetDistance);
		Location.Z += TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), OutLaunchVelocity, GetOwner()->GetActorLocation(),Location, 0.0, BossTargetDistance );
		OwnerCharacter->LaunchCharacter(OutLaunchVelocity, true, true);
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
		OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
		OwnerCharacter->LandedDelegate.RemoveDynamic(this, &UOVAttackComponent::OnCharacterLanded);
	}
}

