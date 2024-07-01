// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/OVDamageComponent.h"

#include "Interface/OVDamagableInterface.h"

UOVDamageComponent::UOVDamageComponent()
{
	bIsInvincible = false;
	bIsDead  = false;
	bIsInterruptible  = true;
	bIsBlocking  = false;
	bIsShieldSkill = false;
}

void UOVDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}


void UOVDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UOVDamageComponent::TakeDamage(FDamageInfo DamageInfo)
{
	EDamageResult DamageResult = CanbeDamage(DamageInfo.ShouldDamageInvincible, DamageInfo.CanBeBlocked);
	switch (DamageResult)
	{
	case EDamageResult::Block:
		OnBlocked.Broadcast(DamageInfo.CanBeBlocked);
		return false;
	case EDamageResult::Damage:
		{
			float NewHp = Health - DamageInfo.Amount;
			Health = FMath::Clamp<float>(NewHp, 0.0f, MaxHealth);
			if (Health <= 0)
			{
				bIsDead = true;
				OnDeath.Broadcast();
				return false;
			}
			if (bIsInterruptible || DamageInfo.ShouldForceInterrupt)
			{
				OnDamageResponse.Broadcast(DamageInfo.DamageResponse);
				return true;
			}
			return true;
		}
	case EDamageResult::NoDamage:
		return false;
	default:
		return false;
	}
}


float UOVDamageComponent::Heal(float Amount)
{
	if(!bIsDead)
	{
		float NewHp = Health + Amount;
		Health = FMath::Clamp<float>(NewHp, 0.0f, MaxHealth);
		return Health;
	}
	return Health;
}

void UOVDamageComponent::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
}

EDamageResult UOVDamageComponent::CanbeDamage(bool ShouldDamageInvincible, bool CanbeBlocked)
{
	if(!bIsDead && (!bIsInvincible || ShouldDamageInvincible))
	{
		if(bIsBlocking && CanbeBlocked)
		{
			return EDamageResult::Block;
		}
		if(bIsShieldSkill)
		{
			return EDamageResult::NoDamage;
		}
		return EDamageResult::Damage;
	}
	return EDamageResult::NoDamage;
}
