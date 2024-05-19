// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "OVAIController.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API AOVAIController : public AAIController
{
	GENERATED_BODY()
public:
	AOVAIController();

	void RunAI();
	void StopAI();
	
protected:
	virtual void OnPossess(APawn* InPawn) override; //컨트롤러가 폰에 빙의해서 조종할 때 발생되는 이벤트 함수


private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
