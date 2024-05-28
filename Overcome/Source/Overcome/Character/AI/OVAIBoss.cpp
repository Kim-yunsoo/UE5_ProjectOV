// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVAIBoss.h"
#include "OVAIBossController.h"
#include "Components/WidgetComponent.h"
#include "Game/OVGameState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Stat/OVCharacterStatComponent.h"
#include "UI/OVBossHpWidget.h"
#include "UI/OVHUDWidget.h"
#include "UI/OVWidgetComponent.h"

AOVAIBoss::AOVAIBoss()
{
	
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.f), FRotator(0.0f, -90.0f, 0.0f));
	// //GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharaterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DarkMetalPack/DarkMetalOniCyclone/Mesh/SM_ONICyclone.SM_ONICyclone'"));
	if (CharaterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharaterMeshRef.Object); 
	}
	Stat->SetIsBoss(true);
	Stat->SetMaxHp(500);

	// HpBar = CreateDefaultSubobject<UOVWidgetComponent>(TEXT("Widget"));
	// HpBar->SetupAttachment(GetMesh());
	// HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	// static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UMG/WBP_HpBar.WBP_HpBar_C"));
	// if (HpBarWidgetRef.Class)
	// {
	// 	HpBar->SetWidgetClass(HpBarWidgetRef.Class);
	// 	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	// 	HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
	// 	HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// }

	Sword = CreateDefaultSubobject<AOVSword>(TEXT("Sword"));
	Sword_l = CreateDefaultSubobject<AOVSword>(TEXT("Sword_l"));
	
	AIControllerClass = AOVAIBossController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	bIsEquipSword = false;
	bIsWieldingWeapon = false;
	bIsFirst = true;
}

void AOVAIBoss::BeginPlay()
{
	Super::BeginPlay();
}

void AOVAIBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOVAIBoss::SetAIDefaultAttackDelegate(const FAIEnemyAttackFinished& InOnAttackFinished)
{
	OnDefaultAttackFinished = InOnAttackFinished;
}

void AOVAIBoss::DefaultAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DefaultAttackMontage, 1.0f);

	//몽타주 완료 델리게이트
	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &AOVAIBoss::OnDefaultAttackMontageEnded);
	AnimInstance->Montage_SetEndDelegate(CompleteDelegate, DefaultAttackMontage);
}

void AOVAIBoss::EauipWeapon()
{
	if (HasAuthority())
	{
		Sword = GetWorld()->SpawnActor<AOVSword>(SwordClass);
		//Sword_l = GetWorld()->SpawnActor<AOVSword>(SwordClass);
		if (!Sword)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Sword"));
		}
		else
		{
			Sword->SetOwner(this);
			//Sword_l->SetOwner(this);
			Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Saber"));
			//Sword_l->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Saber_l"));
			bIsEquipSword = true;
			bIsWieldingWeapon = true;
		}
	}
}

void AOVAIBoss::UneauipWeapon()
{
	//Server
	Sword->Destroy();
	//Destroy(Sword_l);
	bIsEquipSword = false;
	bIsWieldingWeapon = false;
}

float AOVAIBoss::GetIsWieldingWeapon()
{
	return bIsWieldingWeapon;
}

void AOVAIBoss::OnDefaultAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnDefaultAttackFinished.ExecuteIfBound();
}

void AOVAIBoss::SetState(E_AIState AIStateValue)
{
	AIState = AIStateValue;
	if(bIsFirst && AIState == E_AIState::Attacking)
	{
		bIsFirst = false;
		if (AOVGameState* GameState = Cast<AOVGameState>(UGameplayStatics::GetGameState(GetWorld())))
		{
			GameState->BossState(true);
			UE_LOG(LogTemp,Warning, TEXT("SetState BOSS"));
		}
	}
}

E_AIState AOVAIBoss::GetState()
{
	return AIState;
}

// void AOVAIBoss::SetupHUDWidget(UOVHUDWidget* InUserWidget)
// {
// 	UOVBossHpWidget* BossWidget = InUserWidget->BossHpWidget;
// 	if(BossWidget)
// 	{
// 		InUserWidget->UpdateBossUI(false);
// 		UE_LOG(LogTemp, Warning, TEXT("BOSSWidget"));
// 		//StatWidget->UpdateStatWidget(Stat->GetCurrentHp(), Stat->GetCurrentMp(), Stat->GetCurrentAttack());
// 		//Stat->OnStatChanged.AddUObject(StatWidget, &UOVStatWidget::UpdateStatWidget);
// 	}
// }

void AOVAIBoss::SetMovementSpeed(E_MovementSpeed SpeedValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Boss SetMovementSpeed"));

	if(SpeedValue == E_MovementSpeed::Idle)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}
	else if(SpeedValue == E_MovementSpeed::Walking)
	{
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}
	else if(SpeedValue == E_MovementSpeed::Jogging)
	{
		UE_LOG(LogTemp, Warning, TEXT("Jogging"));
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	else if(SpeedValue == E_MovementSpeed::Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 350.f;
	}
}

