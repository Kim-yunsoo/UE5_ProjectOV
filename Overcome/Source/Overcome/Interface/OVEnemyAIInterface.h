// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OVEnemyAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOVEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EMovementSpeed : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Jogging UMETA(DisplayName = "Jogging"),
	Sprinting UMETA(DisplayName = "Sprinting"),
};

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Attacking UMETA(DisplayName = "Attacking"),
	Frozen UMETA(DisplayName = "Frozen"),
	Investigating UMETA(DisplayName = "Investigating"),
	Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class E_BossAttack : uint8
{
	Combo1 UMETA(DisplayName = "Combo1"),
	Combo2 UMETA(DisplayName = "Combo2"),
	JumpAttack UMETA(DisplayName = "JumpAttack"),
	QuickAttack UMETA(DisplayName = "QuickAttack"),
	GroundSmash UMETA(DisplayName = "GroundSmash"),
	AOEAttack UMETA(DisplayName = "AOEAttack")
};

DECLARE_DELEGATE(FAIEnemyAttackFinished);
DECLARE_DELEGATE(FAIEnemyTeleportFinished);

/**
 * 
 */
class OVERCOME_API IOVEnemyAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetIsWieldingWeapon() = 0;
	virtual void SetAIDefaultAttackDelegate(const FAIEnemyAttackFinished& InOnAttackFinished) = 0;
	virtual void SetAITeleportDelegate(const FAIEnemyTeleportFinished& InOnTeleportFinished) = 0;
	virtual void DefaultAttack() = 0;
	virtual void EauipWeapon() = 0;
	virtual void UneauipWeapon() = 0;
	virtual void SetState(EAIState AIStateValue) = 0;
	virtual EAIState GetState() = 0;
	virtual void SetMovementSpeed(EMovementSpeed SpeedValue) = 0;
	virtual void BossAttack(E_BossAttack BossAttack) = 0;
	virtual void SetIsInterruptible(bool bIsInterruptibleValue) = 0;
	virtual void BossTeleport(FVector Location) = 0;
	virtual void SetIsInvincible(bool bIsInvincibleValue) = 0;
};
