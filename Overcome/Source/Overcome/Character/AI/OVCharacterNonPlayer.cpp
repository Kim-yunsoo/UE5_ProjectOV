// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVCharacterNonPlayer.h"

AOVCharacterNonPlayer::AOVCharacterNonPlayer()
{
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
