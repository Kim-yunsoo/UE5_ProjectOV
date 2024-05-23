// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVAIBoss.h"
#include "OVAIBossController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Stat/OVCharacterStatComponent.h"

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
	//Sword
	Sword = CreateDefaultSubobject<AOVSword>(TEXT("Sword"));
	Sword_l = CreateDefaultSubobject<AOVSword>(TEXT("Sword_l"));

	Stat->SetMaxHp(200);

	//구조 짜기 
	// AIControllerClass = AOVAIBossController::StaticClass();
	// AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bIsEquipSword = false;
	bIsWieldingWeapon = false;
}

void AOVAIBoss::BeginPlay()
{
	Super::BeginPlay();
	//Test용
	EauipWeapon();
}

void AOVAIBoss::EauipWeapon()
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

void AOVAIBoss::UneauipWeapon()
{
	//Server
	Sword->Destroy();
	//Destroy(Sword_l);
	bIsEquipSword = false;
	bIsWieldingWeapon = false;

}

