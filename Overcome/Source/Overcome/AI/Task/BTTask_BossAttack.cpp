// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_BossAttack.h"

#include "AIController.h"
#include "Interface/OVEnemyAIInterface.h"

UBTTask_BossAttack::UBTTask_BossAttack()
{
}

EBTNodeResult::Type UBTTask_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	FAIEnemyAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			//UE_LOG(LogTemp, Warning, TEXT("Default Attack End"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //마무리 했다

		}
	);

	AIPawn->SetAIDefaultAttackDelegate(OnAttackFinished);
	AIPawn->BossAttack(BossAttackValue);
	return EBTNodeResult::InProgress;
}
