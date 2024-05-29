// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVAIBoss.h"
#include "OVAIBossController.h"
#include "Game/OVGameState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
	DamageComponent->SetMaxHealth(500);
	
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

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		                                       [&]()
		                                       {
			                                       TestAttack();
		                                       }
	                                       ), 2, false);
	
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

bool AOVAIBoss::TakeDamage(FDamageInfo DamageInfo)
{
	DamageComponent->TakeDamage(DamageInfo);
	Stat->SetHp(DamageComponent->Health);
	return true;
}

void AOVAIBoss::Blocked(bool CanBeParried)
{
	UE_LOG(LogTemp,Warning ,TEXT("Boss Blocked"));
}

void AOVAIBoss::DamageResponse(E_DamageResponses DamageResponses)
{
	UE_LOG(LogTemp,Warning ,TEXT("Boss DamageResponse"));

}

void AOVAIBoss::SetDead()
{
	//Super::SetDead();
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //랙돌 만들기
	UE_LOG(LogTemp,Warning ,TEXT("Boss Dead"));
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
				DamagableInterface->TakeDamage(DamageInfo); //반환값 bool
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
		UE_LOG(LogTemp, Warning, TEXT("Jogging"));
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	else if(SpeedValue == E_MovementSpeed::Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 350.f;
	}
}

