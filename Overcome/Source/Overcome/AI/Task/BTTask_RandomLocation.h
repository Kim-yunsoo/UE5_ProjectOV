// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UBTTask_RandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_RandomLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; 
};
