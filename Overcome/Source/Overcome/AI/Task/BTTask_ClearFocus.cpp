// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_ClearFocus.h"

#include "AIController.h"
#include "Interface/OVEnemyAIInterface.h"

UBTTask_ClearFocus::UBTTask_ClearFocus()
{
}

EBTNodeResult::Type UBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if(nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IOVEnemyAIInterface* AIPawn = Cast<IOVEnemyAIInterface>(ControllingPawn);
	if(nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
	return EBTNodeResult::Succeeded;
}
