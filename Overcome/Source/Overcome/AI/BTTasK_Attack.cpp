// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasK_Attack.h"
#include "AIController.h"
#include "Interface/OVCharacterAIInterface.h"

UBTTasK_Attack::UBTTasK_Attack()
{
}

EBTNodeResult::Type UBTTasK_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IOVCharacterAIInterface* AIPawn = Cast<IOVCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	 FAICharacterAttackFinished OnAttackFinished;
	 OnAttackFinished.BindLambda(
	 	[&]()
	 	{
	 		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //마무리 했다
	 	}
	 );

	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackByAI();
	return EBTNodeResult::InProgress;
}
