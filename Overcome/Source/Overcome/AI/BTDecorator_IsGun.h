// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsGun.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UBTDecorator_IsGun : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_IsGun();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
