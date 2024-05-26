// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTT_SheathSword.h"

#include "AIController.h"
#include "Interface/OVEnemyAIInterface.h"

UBTT_SheathSword::UBTT_SheathSword()
{
}

EBTNodeResult::Type UBTT_SheathSword::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	AIPawn->UneauipWeapon();
	return EBTNodeResult::Succeeded;
}
