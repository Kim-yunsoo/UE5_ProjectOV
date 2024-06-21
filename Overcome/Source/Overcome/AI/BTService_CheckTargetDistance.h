// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckTargetDistance.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UBTService_CheckTargetDistance : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckTargetDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float DistanceThreshold = 700.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float TimeToForgetTarget = 3.0f;

	UPROPERTY()
	float TimeSinceOutOfRange = 0.0f;
};
