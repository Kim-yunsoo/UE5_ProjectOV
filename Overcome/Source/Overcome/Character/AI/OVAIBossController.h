// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "OVAIBoss.h"
#include "OVAIEnemyBaseController.h"
#include "OVAIBossController.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API AOVAIBossController : public AAIController
{
	GENERATED_BODY()
public:
	AOVAIBossController();

	void RunAI();
	void StopAI();
	
protected:
	virtual void OnPossess(APawn* InPawn) override; //컨트롤러가 폰에 빙의해서 조종할 때 발생되는 이벤트 함수

	//void SetPerceptionSystem() override;
private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	//Perception AI
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAIPerceptionComponent> AIPerception;

	UFUNCTION()
	virtual void SetPerceptionSystem();

	UFUNCTION()
	void SetBlackBoardKey();
	
	UFUNCTION()
	void HandleSightSense(AActor *Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void HandleSoundSense(AActor *Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void HandleDamageSense(AActor *Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void HandlePerceptionUpdated(AActor *Actor, FAIStimulus Stimulus);

	UPROPERTY()
	AOVCharacterPlayer* CharacterPlayer;
	

	UPROPERTY()
	AOVAIBoss *Boss;
};
