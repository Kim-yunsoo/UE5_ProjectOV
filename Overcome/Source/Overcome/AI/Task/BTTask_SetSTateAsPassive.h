// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetSTateAsPassive.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UBTTask_SetSTateAsPassive : public UBTTaskNode
{
	GENERATED_BODY()
		
public:
	UBTTask_SetSTateAsPassive();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
