// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_Condition.h"

#include "AIController.h"
#include "Interface/OVEnemyAIInterface.h"


UBTDecorator_Condition::UBTDecorator_Condition()
{
	NodeName = TEXT("BTDecorator_Condition");
}

bool UBTDecorator_Condition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IOVEnemyAIInterface* AIPawn = Cast<IOVEnemyAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	if(AIPawn->GetAIState() == AIState)
	{
		return true;
	}
	else
	{
		return false;
	}
}
