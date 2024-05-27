// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTT_FocusTarget.h"
#include "AIController.h"
#include "AI/OVAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/OVEnemyAIInterface.h"

class IOVEnemyAIInterface;

UBTT_FocusTarget::UBTT_FocusTarget()
{
}

EBTNodeResult::Type UBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
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

	AActor* TargetPawn = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_ATTACKTARGET));
//UE_LOG(LogTemp, Warning, TEXT("%s"), *TargetPawn->GetName())
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	OwnerComp.GetAIOwner()->SetFocus(TargetPawn, EAIFocusPriority::Gameplay);
	return EBTNodeResult::Succeeded;
}
