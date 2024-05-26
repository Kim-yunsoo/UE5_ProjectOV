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
enum class E_MovementSpeed : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Jogging UMETA(DisplayName = "Jogging"),
	Sprinting UMETA(DisplayName = "Sprinting"),
	
};

UENUM(BlueprintType)
enum class E_AIState : uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Attacking UMETA(DisplayName = "Attacking"),
	Frozen UMETA(DisplayName = "Frozen"),
	Investigating UMETA(DisplayName = "Investigating"),
	Dead UMETA(DisplayName = "Dead")
};

DECLARE_DELEGATE(FAIEnemyAttackFinished);
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
	virtual void DefaultAttack() = 0;
	virtual void EauipWeapon() = 0;
	virtual void UneauipWeapon() = 0;

	virtual void SetMovementSpeed(E_MovementSpeed SpeedValue) = 0;

	virtual E_AIState GetAIState() = 0;

};
