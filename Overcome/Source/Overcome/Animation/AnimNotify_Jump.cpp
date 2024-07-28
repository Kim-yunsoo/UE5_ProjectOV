// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_Jump.h"

#include "Character/AI/OVAIBoss.h"

void UAnimNotify_Jump::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AOVAIBoss* BossCharacter = Cast<AOVAIBoss>(MeshComp->GetOwner());
		if (BossCharacter)
		{
			BossCharacter->JumpCheck();
		}
	}
}
