// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SheathSword.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UBTT_SheathSword : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_SheathSword();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
};
