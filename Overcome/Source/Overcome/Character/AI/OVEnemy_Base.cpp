// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVEnemy_Base.h"
#include "OVAIBossController.h"

// Sets default values
AOVEnemy_Base::AOVEnemy_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Stat->SetMaxHp(200);

	AIControllerClass = AOVAIEnemyBaseController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AOVEnemy_Base::BeginPlay()
{
	Super::BeginPlay();
	DefaultAttack();
}

// Called every frame
void AOVEnemy_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOVEnemy_Base::SetAIDefaultAttackDelegate(const FAIEnemyAttackFinished& InOnAttackFinished)
{
	OnDefaultAttackFinished = InOnAttackFinished;
}

void AOVEnemy_Base::DefaultAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("DefaultAttack"));

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DefaultAttackMontage, 1.0f);

	//몽타주 완료 델리게이트
	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &AOVEnemy_Base::OnDefaultAttackMontageEnded);
	AnimInstance->Montage_SetEndDelegate(CompleteDelegate, DefaultAttackMontage);
}

void AOVEnemy_Base::OnDefaultAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnDefaultAttackFinished.ExecuteIfBound();
}


