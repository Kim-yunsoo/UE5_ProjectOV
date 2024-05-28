// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OVGameState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossHpChanged, float, NewHp);

UCLASS()
class OVERCOME_API AOVGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AOVGameState();

	// 보스 HP를 설정하는 함수
	void SetBossHp(float NewHp);

	// 보스 HP를 가져오는 함수
	float GetBossHp() const;

	// 보스 HP 변경 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBossHpChanged OnBossHpChanged;

private:
	float BossHp;
};
