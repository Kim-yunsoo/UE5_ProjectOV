// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_SetIsInvincible.h"

#include "AIController.h"
#include "Interface/OVEnemyAIInterface.h"

UBTTask_SetIsInvincible::UBTTask_SetIsInvincible()
{
}

EBTNodeResult::Type UBTTask_SetIsInvincible::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IOVEnemyAIInterface* AIPawn = Cast<IOVEnemyAIInterface>(ControllingPawn);
	if(nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}
	AIPawn->SetIsInvincible(bIsInvincibleValue);
	return EBTNodeResult::Succeeded;
}
