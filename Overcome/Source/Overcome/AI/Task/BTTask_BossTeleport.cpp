// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_BossTeleport.h"

#include "AIController.h"
#include "AI/OVAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/OVEnemyAIInterface.h"

UBTTask_BossTeleport::UBTTask_BossTeleport()
{
}

EBTNodeResult::Type UBTTask_BossTeleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

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
	
	FAIEnemyTeleportFinished OnTeleportFinished;
	OnTeleportFinished.BindLambda(
		[&]()
		{
			//UE_LOG(LogTemp, Warning, TEXT("BossTelePortEnd"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //마무리 했다
		}
	);
	FVector Location = {0,0,150};
	//UE_LOG(LogTemp, Warning, TEXT("Location True"));
	AActor* AttackTarget = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_ATTACKTARGET));
	if (AttackTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Location True"));
		Location = AttackTarget->GetActorLocation();
	}
	AIPawn->SetAITeleportDelegate(OnTeleportFinished);
	AIPawn->BossTeleport(Location);
	return EBTNodeResult::InProgress;
}
