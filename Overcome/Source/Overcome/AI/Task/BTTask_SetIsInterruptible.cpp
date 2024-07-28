// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_SetIsInterruptible.h"

#include "AIController.h"
#include "Interface/OVEnemyAIInterface.h"

UBTTask_SetIsInterruptible::UBTTask_SetIsInterruptible()
{
}

EBTNodeResult::Type UBTTask_SetIsInterruptible::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	AIPawn->SetIsInterruptible(bIsInterruptibleValue);
	return EBTNodeResult::Succeeded;
}
