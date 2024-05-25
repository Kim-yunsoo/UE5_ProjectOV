// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_WieldSword.h"

#include "AIController.h"
#include "Character/AI/OVEnemy_Base.h"



EBTNodeResult::Type UBTTask_WieldSword::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	AIPawn->EauipWeapon();
	return EBTNodeResult::Succeeded;
}

UBTTask_WieldSword::UBTTask_WieldSword()
{
}
