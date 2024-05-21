// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTasK_Attack.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UBTTasK_Attack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTasK_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
