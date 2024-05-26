// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Interface/OVEnemyAIInterface.h"
#include "BTTask_SetMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UBTTask_SetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetMovementSpeed();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	E_MovementSpeed CharacterSpeed;
	
};
