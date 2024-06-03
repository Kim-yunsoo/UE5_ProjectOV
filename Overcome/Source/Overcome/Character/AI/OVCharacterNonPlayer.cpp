// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVCharacterNonPlayer.h"

#include "BrainComponent.h"
#include "OVAIController.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Object/OVPickup.h"
#include "Stat/OVAttackComponent.h"
#include "Stat/OVCharacterStatComponent.h"
#include "Stat/OVDamageComponent.h"
#include "UI/OVWidgetComponent.h"

AOVCharacterNonPlayer::AOVCharacterNonPlayer()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.f), FRotator(0.0f, -90.0f, 0.0f));
	// //GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
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
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/DarkMetalPack/ZombotMike/Animations/Anim_Zombot_Mikey_ClawRT_Montage.Anim_Zombot_Mikey_ClawRT_Montage'"));
	if (AttackMontageRef.Object)
	{
		AttackMontage = AttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StaggerMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/DarkMetalPack/ZombotMike/Animations/Anim_Zombot_Mikey_HitRegister_Montage.Anim_Zombot_Mikey_HitRegister_Montage'"));
	if (StaggerMontageRef.Object)
	{
		StaggerMontage = StaggerMontageRef.Object;
	}
	
	AIControllerClass = AOVAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HpBar = CreateDefaultSubobject<UOVWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UMG/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	DamageComponent = CreateDefaultSubobject<UOVDamageComponent>(TEXT("DamageComponent"));
	AttackComponent = CreateDefaultSubobject<UOVAttackComponent>(TEXT("AttackComponent"));
	Stat->SetMaxHp(50);
	DamageComponent->SetMaxHealth(50);
	bIsAttacking = false;
	RandomItemName = GetRandomItemName();
	if(RandomItemName == "None")
	{
		UE_LOG(LogTemp,Warning, TEXT("NameNone"));
	}
}

void AOVCharacterNonPlayer::SetDead()
{
	Super::SetDead();
	HpBar->SetHiddenInGame(true);
	PlayDeadAnimation();
	FTimerHandle DeadTimerHandle;
	AOVAIController* AIController = Cast<AOVAIController>(GetController());
	AIController->GetBrainComponent()->StopLogic(TEXT("Dead"));

	FActorSpawnParameters SpawnParams;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *RandomItemName);
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	AOVPickup* PotionObject = GetWorld()->SpawnActor<AOVPickup>(AOVPickup::StaticClass(), GetActorLocation(),
																GetActorRotation(), SpawnParams);
	PotionObject->DesiredItemID = *RandomItemName;
	PotionObject->InitialStart();

	
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
}

void AOVCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	DamageComponent->OnDeath.AddUObject(this, &AOVCharacterNonPlayer::SetDead);
	DamageComponent->OnDamageResponse.AddUObject(this, &AOVCharacterNonPlayer::DamageResponse);
}

float AOVCharacterNonPlayer::GetCurrentHealth()
{
	return Stat->GetCurrentHp();
}

float AOVCharacterNonPlayer::GetMaxHealth()
{
	return Stat->GetMaxHp();
}

float AOVCharacterNonPlayer::Heal(float Amount)
{
	return -1;
}

bool AOVCharacterNonPlayer::TakeDamage(FDamageInfo DamageInfo)
{
	DamageComponent->TakeDamage(DamageInfo);
	Stat->SetHp(DamageComponent->Health);
	return true;
}

bool AOVCharacterNonPlayer::IsDead()
{
	return DamageComponent->bIsDead;
}

bool AOVCharacterNonPlayer::IsAttacking()
{
	return bIsAttacking;
}

void AOVCharacterNonPlayer::DamageResponse(E_DamageResponses DamageResponses)
{
	//UE_LOG(LogTemp,Warning ,TEXT("Mini AI DamageResponse"));
	GetCharacterMovement()->StopMovementImmediately();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(StaggerMontage, 1.0f);
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
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(AttackMontage, 1.0f);
	
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

FString AOVCharacterNonPlayer::GetRandomItemName()
{
	// 0에서 2 사이의 랜덤 값을 생성
	int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, 2);

	// E_Item 열거형 값을 배열에 저장
	E_Item Items[] = { E_Item::HPPotion, E_Item::MPPotion, E_Item::AttackPotion };

	// 선택된 랜덤 값에 해당하는 E_Item 값을 반환
	E_Item SelectedItem = Items[RandomIndex];
	UE_LOG(LogTemp, Warning, TEXT(" RandomIndex %d"), RandomIndex);
	// 선택된 E_Item 값을 문자열로 변환
	switch (SelectedItem)
	{
	case E_Item::HPPotion:
		return TEXT("HPPotion");
	case E_Item::MPPotion:
		return TEXT("MPPotion");
	case E_Item::AttackPotion:
		return TEXT("AttackPotion");
	default:
		return TEXT("None");
	}
}