// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVAIBoss.h"

#include "BrainComponent.h"
#include "NavigationSystem.h"
#include "OVAIBossController.h"
#include "AI/OVAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/OVGameState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Stat/OVAttackComponent.h"
#include "Stat/OVCharacterStatComponent.h"
#include "Stat/OVDamageComponent.h"

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
	DamageComponent = CreateDefaultSubobject<UOVDamageComponent>(TEXT("DamageComponent"));
	AttackComponent = CreateDefaultSubobject<UOVAttackComponent>(TEXT("AttackComponent"));

	DamageComponent->SetMaxHealth(500);
	
	Sword = CreateDefaultSubobject<AOVSword>(TEXT("Sword"));
	Sword_l = CreateDefaultSubobject<AOVSword>(TEXT("Sword_l"));
	
	AIControllerClass = AOVAIBossController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	bIsEquipSword = false;
	bIsWieldingWeapon = false;
	bIsFirst = true;
	bIsAttacking = false;
	bIsTeleporting = false;

}

void AOVAIBoss::BeginPlay()
{
	Super::BeginPlay();
	BossController = Cast<AOVAIBossController>(GetController());
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
	DamageComponent->bIsInterruptible = false;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DefaultAttackMontage, 1.0f);

	//몽타주 완료 델리게이트
	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &AOVAIBoss::OnDefaultAttackMontageEnded);
	AnimInstance->Montage_SetEndDelegate(CompleteDelegate, DefaultAttackMontage);
}

void AOVAIBoss::BossAttack(E_BossAttack BossAttack)
{
	if(BossAttack == E_BossAttack::Combo1)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ATTACKCOMO1"));
		AttackCombo1();
	}
	else if(BossAttack == E_BossAttack::Combo2)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ATTACKCOMO1"));
		AttackCombo2();
	}
}

void AOVAIBoss::EauipWeapon()
{
	if (HasAuthority())
	{
		Sword = GetWorld()->SpawnActor<AOVSword>(SwordClass);
		//Sword_l = GetWorld()->SpawnActor<AOVSword>(SwordClass);
		if (!Sword)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Not Sword"));
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
	DamageComponent->bIsInterruptible = true;
	OnDefaultAttackFinished.ExecuteIfBound();
	//Attack End
	//UE_LOG(LogTemp, Warning, TEXT("MontageEnded"));
}

void AOVAIBoss::OnStaggerMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	SetState(E_AIState::Attacking);
	BossController->GetBlackboardComponent()->SetValueAsEnum(BBKEY_STATE,static_cast<uint8>(GetState()));
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
			//UE_LOG(LogTemp,Warning, TEXT("SetState BOSS"));
		}
	}
}

E_AIState AOVAIBoss::GetState()
{
	return AIState;
}

void AOVAIBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	DamageComponent->OnDeath.AddUObject(this, &AOVAIBoss::SetDead);
	DamageComponent->OnBlocked.AddUObject(this, &AOVAIBoss::Blocked);
	DamageComponent->OnDamageResponse.AddUObject(this, &AOVAIBoss::DamageResponse);
}

float AOVAIBoss::GetCurrentHealth()
{
	return Stat->GetCurrentHp();
}

float AOVAIBoss::GetMaxHealth()
{
	return Stat->GetMaxHp();
}

float AOVAIBoss::Heal(float Amount)
{
	DamageComponent->Heal(Amount);
	Stat->SetHp(DamageComponent->Health);
	return DamageComponent->Health;
}

bool AOVAIBoss::TakeDamage(FDamageInfo DamageInfo) //영향 받음
{
	//UE_LOG(LogTemp, Warning, TEXT("TakeDamage"));
	DamageComponent->TakeDamage(DamageInfo);
	Stat->SetHp(DamageComponent->Health);
	return true;
}

bool AOVAIBoss::IsDead()
{
	return DamageComponent->bIsDead;
}

bool AOVAIBoss::IsAttacking()
{
	return bIsAttacking;
}

void AOVAIBoss::SetIsInterruptible(bool bIsInterruptibleValue)
{
	DamageComponent->bIsInterruptible = bIsInterruptibleValue;
}

void AOVAIBoss::BossTeleport(FVector Location)
{
	if(!bIsTeleporting)
	{
		bIsTeleporting = true;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->MaxFlySpeed = 5000;
		GetCharacterMovement()->MaxAcceleration = 99999;
		GetMesh()->SetVisibility(false, true);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		//UE_LOG(LogTemp, Warning, TEXT("Location %s"), *Location.ToString());
		//if( BossController->MoveToLocation(Location, 150, false, true, true, false, nullptr , true) == EPathFollowingRequestResult::RequestSuccessful)
		//SetActorLocation(Location);
		FNavLocation RandomNavLocation;
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(Location, 130, RandomNavLocation))
		{
			SetActorLocation(RandomNavLocation.Location);
			BossTeleportEnd();
		}
	}
	else 
	{
		//TelePort END 몇초 후
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = FName("OnTeleportEnd");
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = __LINE__;
		
		UKismetSystemLibrary::DelayUntilNextTick(this, LatentInfo);
		//보스 행동트리 끝
		OnTeleportFinished.ExecuteIfBound();
	}
}

void AOVAIBoss::SetAITeleportDelegate(const FAIEnemyTeleportFinished& InOnTeleportFinished)
{
	OnTeleportFinished = InOnTeleportFinished;
	//UE_LOG(LogTemp,Warning, TEXT("SetAITeleportDelegate"));
}

void AOVAIBoss::BossTeleportEnd()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->MaxAcceleration = 1500;
	GetMesh()->SetVisibility(true, true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	OnTeleportFinished.ExecuteIfBound();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		bIsTeleporting = false;
	}, 0.5f, false);
}

void AOVAIBoss::Blocked(bool CanBeParried)
{
	//UE_LOG(LogTemp,Warning ,TEXT("Boss Blocked"));
}

void AOVAIBoss::DamageResponse(E_DamageResponses DamageResponses)
{
	//UE_LOG(LogTemp,Warning ,TEXT("Boss DamageResponse"));
	//if(GetState() != E_AIState::Frozen) //공격 받고 있지 않을때 공격 들어감!
	{
		GetCharacterMovement()->StopMovementImmediately();
		SetState(E_AIState::Frozen);
		BossController->GetBlackboardComponent()->SetValueAsEnum(BBKEY_STATE,static_cast<uint8>(GetState()));
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->StopAllMontages(0.0f);
		AnimInstance->Montage_Play(StaggerMontage, 1.0f);

		FOnMontageEnded CompleteDelegate;
		//CompleteDelegate.BindUObject(this, &AOVAIBoss::OnDefaultAttackMontageEnded);
		CompleteDelegate.BindUObject(this, &AOVAIBoss::OnStaggerMontageEnded);
		AnimInstance->Montage_SetEndDelegate(CompleteDelegate, StaggerMontage);
	}
}

void AOVAIBoss::SetDead()
{
	//Super::SetDead();
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //랙돌 만들기
	BossController->GetBrainComponent()->StopLogic(TEXT("Dead"));
	SetState(E_AIState::Dead);
	BossController->GetBlackboardComponent()->SetValueAsEnum(BBKEY_STATE,static_cast<uint8>(GetState()));
	//UE_LOG(LogTemp,Warning ,TEXT("Boss Dead"));
}


void AOVAIBoss::SlashCheck()
{
	if(!DamageComponent->bIsDead)
	{
		FDamageInfo DamageInfo = {20, E_DamageType::Melee, E_DamageResponses::HitReaction, false, true, true, false };
		AttackComponent->AttackSlash(40, 220, DamageInfo);
	}
}

void AOVAIBoss::AOESlashCheck()
{
	if (!DamageComponent->bIsDead)
	{
		FDamageInfo DamageInfo = {20, E_DamageType::Explosion, E_DamageResponses::HitReaction, false, false, false, true};
		AttackComponent->AttackAOESlash(300, DamageInfo);
	}
}

void AOVAIBoss::AttackCombo1()
{
	UE_LOG(LogTemp, Warning, TEXT("ATTACKCOMO1"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(AttackCombo1Montage, 1.0f);

	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &AOVAIBoss::OnDefaultAttackMontageEnded);
	AnimInstance->Montage_SetEndDelegate(CompleteDelegate, AttackCombo1Montage);
}

void AOVAIBoss::AttackCombo2()
{
	UE_LOG(LogTemp, Warning, TEXT("ATTACKCOMO2"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(AttackCombo2Montage, 1.0f);

	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &AOVAIBoss::OnDefaultAttackMontageEnded);
	AnimInstance->Montage_SetEndDelegate(CompleteDelegate, AttackCombo2Montage);
}

void AOVAIBoss::TestAttack()
{
	if(!DamageComponent->bIsDead)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectArray;
		ObjectArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		TArray<AActor*> ActorsToNotTargeting;
		ActorsToNotTargeting.Add(this);
		FVector Start = GetActorLocation();
		FVector End = GetActorForwardVector() * 100 + Start;
		FHitResult HitResult;
		bool bResult = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),Start, End, 20, ObjectArray, false,ActorsToNotTargeting
		,EDrawDebugTrace::ForDuration, HitResult,  true,
				FLinearColor::Red, FLinearColor::Yellow, 1.f);

		//테스트 공격 100 
		FDamageInfo DamageInfo = {50, E_DamageType::Melee, E_DamageResponses::HitReaction, false, false, false, false };
		if(bResult)
		{
			IOVDamagableInterface* DamagableInterface = Cast<IOVDamagableInterface>(HitResult.GetActor());
			if(DamagableInterface)
			{
				//DamagableInterface->TakeDamage(DamageInfo); //반환값 bool
			}
		}
	}
	
}

void AOVAIBoss::SetMovementSpeed(E_MovementSpeed SpeedValue)
{
	//UE_LOG(LogTemp, Warning, TEXT("Boss SetMovementSpeed"));

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
	//UE_LOG(LogTemp, Warning, TEXT("Jogging"));
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	else if(SpeedValue == E_MovementSpeed::Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 350.f;
	}
}

