// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateDistanceToTarget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UBTService_UpdateDistanceToTarget : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_UpdateDistanceToTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
