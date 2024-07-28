// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_SetMovementSpeed.h"

#include "AIController.h"
#include "Character/AI/OVAIEnemyBaseController.h"
#include "Interface/OVEnemyAIInterface.h"

UBTTask_SetMovementSpeed::UBTTask_SetMovementSpeed()
{
	
}

EBTNodeResult::Type UBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IOVEnemyAIInterface* AIPawn = Cast<IOVEnemyAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	AIPawn->SetMovementSpeed(CharacterSpeed);
	return EBTNodeResult::Succeeded;
}
