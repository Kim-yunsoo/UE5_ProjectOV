// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVCharacterNonPlayer.h"

#include "OVAIController.h"

AOVCharacterNonPlayer::AOVCharacterNonPlayer()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.f), FRotator(0.0f, -90.0f, 0.0f));
	// //GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharaterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DarkMetalPack/ZombotMike/Mesh/SM_Zombot_Mikey.SM_Zombot_Mikey'"));
	if (CharaterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharaterMeshRef.Object); 
	}

	AIControllerClass = AOVAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; 
}

void AOVCharacterNonPlayer::SetDead()
{
	Super::SetDead();
	
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}
