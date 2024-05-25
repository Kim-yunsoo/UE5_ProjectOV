// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVEnemy_Base.h"
#include "OVAIBossController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sword/OVSword.h"

// Sets default values
AOVEnemy_Base::AOVEnemy_Base()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharaterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DarkMetalPack/DarkMetalOniCyclone/Mesh/SM_ONICyclone.SM_ONICyclone'"));
	if (CharaterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharaterMeshRef.Object); 
	}
	//Sword
	Sword = CreateDefaultSubobject<AOVSword>(TEXT("Sword"));
	Sword_l = CreateDefaultSubobject<AOVSword>(TEXT("Sword_l"));

		
	AIControllerClass = AOVAIEnemyBaseController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	bIsEquipSword = false;
	bIsWieldingWeapon = false;

	//Stat->SetMaxHp(200);

}

// Called when the game starts or when spawned
void AOVEnemy_Base::BeginPlay()
{
	Super::BeginPlay();
	//DefaultAttack();
	//EauipWeapon();
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

void AOVEnemy_Base::EauipWeapon()
{
	//Server
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

void AOVEnemy_Base::UneauipWeapon()
{
	//Server
	Sword->Destroy();
	//Destroy(Sword_l);
	bIsEquipSword = false;
	bIsWieldingWeapon = false;
}

float AOVEnemy_Base::GetIsWieldingWeapon()
{
	return bIsWieldingWeapon;
}

void AOVEnemy_Base::OnDefaultAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnDefaultAttackFinished.ExecuteIfBound();
}
