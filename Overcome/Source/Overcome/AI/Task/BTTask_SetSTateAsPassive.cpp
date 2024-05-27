// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_SetSTateAsPassive.h"

#include "Character/AI/OVAIEnemyBaseController.h"
#include "Interface/OVEnemyAIInterface.h"

UBTTask_SetSTateAsPassive::UBTTask_SetSTateAsPassive()
{
}

EBTNodeResult::Type UBTTask_SetSTateAsPassive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);


	AOVAIEnemyBaseController* AIController = Cast<AOVAIEnemyBaseController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	AIController->SetState(E_AIState::Passive);
	return EBTNodeResult::Succeeded;

}
