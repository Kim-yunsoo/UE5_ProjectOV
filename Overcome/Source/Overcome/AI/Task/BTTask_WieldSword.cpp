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

	AOVEnemy_Base * Enemy_Base = Cast<AOVEnemy_Base>(ControllingPawn);

	Enemy_Base->EauipWeapon();
	return EBTNodeResult::Succeeded;
}

UBTTask_WieldSword::UBTTask_WieldSword()
{
}
