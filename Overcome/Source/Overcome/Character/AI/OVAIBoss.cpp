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
	
	Stat->SetMaxHp(200);

	AIControllerClass = AOVAIBossController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void AOVAIBoss::BeginPlay()
{
	Super::BeginPlay();
	
	Sword = GetWorld()->SpawnActor<AOVSword>(SwordClass);
	Sword->SetOwner(this);
	Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Back_Socket"));
}

