// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_RunAwayFromPlayer.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "OVAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/OVCharacterPlayer.h"

UBTTaskNode_RunAwayFromPlayer::UBTTaskNode_RunAwayFromPlayer()
{
	NodeName = "DetermineEscapeLocation";
}

EBTNodeResult::Type UBTTaskNode_RunAwayFromPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControllingPawn = AIController->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_CHARACTER);
	AOVCharacterPlayer* TargetActor = Cast<AOVCharacterPlayer>(TargetObject);
	if (TargetActor)
	{
		FVector RunDirection = ControllingPawn->GetActorLocation() - TargetActor->GetActorLocation();
		RunDirection.Normalize();
		FVector RunLocation = ControllingPawn->GetActorLocation() + RunDirection * SafeDistance;

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(ControllingPawn->GetWorld());
		if (NavSystem)
		{
			FNavLocation NavLocation;
			if (NavSystem->GetRandomPointInNavigableRadius(RunLocation, SafeDistance, NavLocation))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_RUNLOCATION, NavLocation.Location);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
