// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_WieldSword.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UBTTask_WieldSword : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_WieldSword();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
