// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVCharacterNonPlayer.h"

#include "OVAIController.h"
#include "Stat/OVCharacterStatComponent.h"

AOVCharacterNonPlayer::AOVCharacterNonPlayer()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.f), FRotator(0.0f, -90.0f, 0.0f));
	// //GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharaterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DarkMetalPack/ZombotMike/Mesh/SM_Zombot_Mikey.SM_Zombot_Mikey'"));
	if (CharaterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharaterMeshRef.Object); 
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/DarkMetalPack/ZombotMike/Animations/Anim_Zombot_Mikey_Death_Montage.Anim_Zombot_Mikey_Death_Montage'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}
	
	
	AIControllerClass = AOVAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; 
}

void AOVCharacterNonPlayer::SetDead()
{
	Super::SetDead();
	PlayDeadAnimation();
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AOVCharacterNonPlayer::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void AOVCharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();

	Stat->SetMaxHp(50);
}

float AOVCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.f;
}

float AOVCharacterNonPlayer::GetAIDetectRange()
{
	return 400.f;
}

float AOVCharacterNonPlayer::GetAIAttackRange()
{
	return 100.f; // 공격 범위 정하자
}

float AOVCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AOVCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AOVCharacterNonPlayer::AttackByAI()
{
	//ProcessComboCommand();
	//1초뒤 공격 끝난다
	UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			NotifyActionEnd();
		}
	), 1.0f, false);
}

void AOVCharacterNonPlayer::NotifyActionEnd()
{
	Super::NotifyActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
