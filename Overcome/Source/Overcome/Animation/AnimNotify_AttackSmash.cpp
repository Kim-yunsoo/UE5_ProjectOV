// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackSmash.h"

#include "Character/AI/OVAIBoss.h"

void UAnimNotify_AttackSmash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AOVAIBoss* BossCharacter = Cast<AOVAIBoss>(MeshComp->GetOwner());
		if (BossCharacter)
		{
			BossCharacter->SmashCheck(); //공격 판정 해라!
		}
	}
}
