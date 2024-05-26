// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_Condition.h"

#include "AIController.h"
#include "Character/AI/OVAIEnemyBaseController.h"
#include "Interface/OVEnemyAIInterface.h"


UBTDecorator_Condition::UBTDecorator_Condition()
{
	NodeName = TEXT("BTDecorator_Condition");
	bNotifyTick = true;
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

	AOVAIEnemyBaseController* AIController = Cast<AOVAIEnemyBaseController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return false; 
	}
	
	if(AIController->AIState == AIStateCondition)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UBTDecorator_Condition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bool bConditionMet = CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bConditionMet)
	{
		OwnerComp.RequestExecution(this);
	}
}
