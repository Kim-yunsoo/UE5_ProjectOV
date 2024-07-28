// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_UpdateDistanceToTarget.h"

#include "AIController.h"
#include "AI/OVAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/OVCharacterPlayer.h"
#include "Interface/OVEnemyAIInterface.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_UpdateDistanceToTarget::UBTService_UpdateDistanceToTarget()
{
	NodeName = TEXT("UpdateDistanceToTarget");
	Interval = 0.1f;
}

void UBTService_UpdateDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	IOVEnemyAIInterface* AIPawn = Cast<IOVEnemyAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return ;
	}

	AOVCharacterPlayer* TargetActor = Cast<AOVCharacterPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_ATTACKTARGET));
	if(TargetActor == nullptr)
	{
		return ;
	}

	float DistanceToTarget = UKismetMathLibrary::Vector_Distance(ControllingPawn->GetActorLocation(), TargetActor->GetActorLocation());

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_DISTANCETARGET, DistanceToTarget);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), DistanceToTarget);
}
