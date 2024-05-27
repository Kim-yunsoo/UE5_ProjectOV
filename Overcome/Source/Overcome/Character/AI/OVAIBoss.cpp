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
}

void AOVAIBoss::BeginPlay()
{
	Super::BeginPlay();

}

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

