// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OVGameState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossHpChanged, float, NewHp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossAttackStateChanged, bool, bIsAttacking);

UCLASS()
class OVERCOME_API AOVGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AOVGameState();

	void SetBossHp(float NewHp);

	float GetBossHp() const;
	
	void SetCharacterDead(bool NewDead);

	bool GetCharacterDead() const;

	void BossState(bool start);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBossHpChanged OnBossHpChanged;

	//공격 상태인지
	FOnBossAttackStateChanged OnBossAttackState;

	UPROPERTY()
	uint8 BossDead : 1;
	
private:
	float BossHp;

	bool BossStart;

	UPROPERTY()
	uint8 CharacterDead : 1;
	

};
