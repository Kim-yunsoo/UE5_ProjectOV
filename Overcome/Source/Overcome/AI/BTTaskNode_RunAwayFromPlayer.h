// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_RunAwayFromPlayer.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UBTTaskNode_RunAwayFromPlayer : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_RunAwayFromPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditDefaultsOnly)
	float SafeDistance = 500.0f;
};
