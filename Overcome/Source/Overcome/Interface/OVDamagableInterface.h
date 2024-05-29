// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OVDamagableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOVDamagableInterface : public UInterface
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class E_DamageType : uint8
{
	None UMETA(DisplayName = "None"),
	Melee UMETA(DisplayName = "Melee"),
	Projectile UMETA(DisplayName = "Projectile"),
	Explosion UMETA(DisplayName = "Explosion"),
	Environment UMETA(DisplayName = "Environment")
};

UENUM(BlueprintType)
enum class E_DamageResponses : uint8
{
	None UMETA(DisplayName = "None"),
	HitReaction UMETA(DisplayName = "Melee"),
	Stagger UMETA(DisplayName = "Projectile"), 
	Stun UMETA(DisplayName = "Explosion"),
	KnockBack UMETA(DisplayName = "Environment")
};

USTRUCT(Atomic, BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float Amount;
	E_DamageType DamageType;
	E_DamageResponses DamageResponse;
	bool ShouldDamageInvincible; //무적상태
	bool CanBeBlocked;
	bool CanBeParried;
	bool ShouldForceInterrupt;
};

/**
 * 
 */
class OVERCOME_API IOVDamagableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetCurrentHealth() = 0;
	virtual float GetMaxHealth() = 0;
	virtual float Heal(float Amount) = 0; //안쓸지도?
	virtual bool TakeDamage(FDamageInfo DamageInfo)  = 0;
	virtual bool IsDead() = 0;
};
