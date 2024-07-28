// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_RandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/OVAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/OVEnemyAIInterface.h"

UBTTask_RandomLocation::UBTTask_RandomLocation()
{
	
}

EBTNodeResult::Type UBTTask_RandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}
	IOVEnemyAIInterface* AIPawn = Cast<IOVEnemyAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	float PatrolRadius = 1000;
	FNavLocation NextPatrolPos;

	if (NavSystem->GetRandomPointInNavigableRadius(ControllingPawn->GetActorLocation(), PatrolRadius, NextPatrolPos)) //반경 안으로 들어오면 성공
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_BOSSPATROLPOS, NextPatrolPos.Location);
		//UE_LOG(LogTemp, Warning, TEXT("%s"), NextPatrolPos);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
