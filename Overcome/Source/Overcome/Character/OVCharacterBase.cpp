// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OVCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "OVCharacterControlData.h"
#include "Components/WidgetComponent.h"
#include "Stat/OVCharacterStatComponent.h"
#include "UI/OVWidgetComponent.h"
#include "UI/OVHpBarWidget.h"

// Sets default values
AOVCharacterBase::AOVCharacterBase()
{
	//Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 350.0f;
	GetCharacterMovement()->AirControl = 0.4f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//GetCharacterMovement()->bRunPhysicsWithNoController = true;

	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharaterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/Belica/Skins/PolarStrike/Meshes/Belica_PolarStrike.Belica_PolarStrike'"));
	if (CharaterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharaterMeshRef.Object); //분수대 만들었을 때처럼 오브젝트 지정
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Blueprint/ABP_OV_Character.ABP_OV_Character_C"));
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class); //클래스 정보 가지고 와서 지정
	}

	static ConstructorHelpers::FObjectFinder<UOVCharacterControlData> ShoulderDataRef(TEXT("/Script/Overcome.OVCharacterControlData'/Game/CharacterControl/ABC_Shouler.ABC_Shouler'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UOVCharacterControlData> QuaterDataRef(TEXT("/Script/Overcome.OVCharacterControlData'/Game/CharacterControl/ABC_Quater.ABC_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	
	//Stat Component
	Stat = CreateDefaultSubobject<UOVCharacterStatComponent>(TEXT("Stat"));

	//Widget Component
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
}

void AOVCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Stat->OnHpZero.AddUObject(this, &AOVCharacterBase::SetDead);
}

void AOVCharacterBase::SetCharacterControlData(const UOVCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

float AOVCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageTpApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Stat->ApplyDamage(DamageAmount);
	return DamageTpApply;
}

void AOVCharacterBase::SetDead()
{
	UE_LOG(LogTemp, Warning, TEXT("DEAD!!!"));
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
	// Destroy(this);
	//SetActorHiddenInGame(true);
}

void AOVCharacterBase::SetupCharacterWidget(UOVUserWidget* InUserWidget)
{
	UOVHpBarWidget* HpBarWidget = Cast<UOVHpBarWidget>(InUserWidget);
	if(HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpchanged.AddUObject(HpBarWidget, &UOVHpBarWidget::UpdateHpBar);
	}
}


