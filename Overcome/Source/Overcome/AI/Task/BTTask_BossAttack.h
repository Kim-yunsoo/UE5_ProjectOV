// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Interface/OVEnemyAIInterface.h"
#include "BTTask_BossAttack.generated.h"

enum class E_AIState : uint8;
/**
 * 
 */
UCLASS()
class OVERCOME_API UBTTask_BossAttack : public UBTTaskNode
{
	GENERATED_BODY()
		
public:
	UBTTask_BossAttack();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	E_BossAttack BossAttackValue;
	
};
