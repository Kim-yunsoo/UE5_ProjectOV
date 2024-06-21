// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckTargetDistance.h"

#include "AIController.h"
#include "OVAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckTargetDistance::UBTService_CheckTargetDistance()
{
	NodeName = TEXT("Check Target Distance");
}

void UBTService_CheckTargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		APawn* AIPawn = AIController->GetPawn();
		if (AIPawn)
		{
			UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);
			AActor* TargetActor = Cast<AActor>(TargetObject);

			if (TargetActor)
			{
				float Distance = FVector::Dist(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());
				if (Distance > DistanceThreshold)
				{
					TimeSinceOutOfRange += DeltaSeconds;
					//UE_LOG(LogTemp, Warning, TEXT("Distance Over %f"), TimeSinceOutOfRange);
					if (TimeSinceOutOfRange >= TimeToForgetTarget)
					{
						OwnerComp.GetBlackboardComponent()->ClearValue(BBKEY_TARGET);
					}
				}
				else
				{
					TimeSinceOutOfRange = 0.0f;
				}
			}
		}
	}
}
