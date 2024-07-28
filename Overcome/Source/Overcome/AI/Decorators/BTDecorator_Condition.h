// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_Condition.generated.h"

enum class EAIState : uint8;
/**
 * 
 */
UCLASS()
class OVERCOME_API UBTDecorator_Condition : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_Condition();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere)
	EAIState AIStateCondition;
};
