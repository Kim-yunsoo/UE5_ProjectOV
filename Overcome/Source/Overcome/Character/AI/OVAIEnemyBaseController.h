// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "OVAIEnemyBaseController.generated.h"

enum class E_AIState : uint8;
/**
 * 
 */
UCLASS()
class OVERCOME_API AOVAIEnemyBaseController : public AAIController
{
	GENERATED_BODY()
public:
	AOVAIEnemyBaseController();

	void RunAI();
	void StopAI();

	UPROPERTY(EditAnywhere)
	E_AIState AIState;
	
	UFUNCTION()
	void SetState(E_AIState AIStateValue);
	
protected:
	virtual void OnPossess(APawn* InPawn) override; 

protected:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	//Perception AI
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAIPerceptionComponent> AIPerception;

	UFUNCTION()
	void SetPerceptionSystem();

	UFUNCTION()
	void HandleSightSense(AActor *Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void HandleSoundSense(AActor *Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void HandleDamageSense(AActor *Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void HandlePerceptionUpdated(AActor *Actor, FAIStimulus Stimulus);

};
