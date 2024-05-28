// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_SetSTateAsPassive.h"

#include "AI/OVAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/OVAIBoss.h"
#include "Character/AI/OVAIBossController.h"
#include "Character/AI/OVAIEnemyBaseController.h"
#include "Interface/OVEnemyAIInterface.h"

UBTTask_SetSTateAsPassive::UBTTask_SetSTateAsPassive()
{
}

EBTNodeResult::Type UBTTask_SetSTateAsPassive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	IOVEnemyAIInterface* AIPawn = Cast<IOVEnemyAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}
	AIPawn->SetState(E_AIState::Passive);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(BBKEY_STATE,static_cast<uint8>(AIPawn->GetState()));
	return EBTNodeResult::Succeeded;

}
