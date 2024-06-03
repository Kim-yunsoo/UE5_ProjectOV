// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/OVDamageComponent.h"

#include "Interface/OVDamagableInterface.h"

UOVDamageComponent::UOVDamageComponent()
{
	bIsInvincible = false;
	bIsDead  = false;
	bIsInterruptible  = true;
	bIsBlocking  = false;
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
	if(CanbeDamage(DamageInfo.ShouldDamageInvincible, DamageInfo.CanBeBlocked) == 0)
	{
		OnBlocked.Broadcast(DamageInfo.CanBeBlocked);
		return false;
	}
	if(CanbeDamage(DamageInfo.ShouldDamageInvincible, DamageInfo.CanBeBlocked) == 1)
	{
		float NewHp = Health - DamageInfo.Amount;
		Health = FMath::Clamp<float>(NewHp, 0.0f, MaxHealth);
		if(Health <= 0)
		{
			bIsDead = true;
			OnDeath.Broadcast(); //죽음 알리기!
			return true;
		}
		if(bIsInterruptible || DamageInfo.ShouldForceInterrupt)
		{
			//Damage Response 출력
			OnDamageResponse.Broadcast(DamageInfo.DamageResponse);
			return true;
		}
	}
	else if(CanbeDamage(DamageInfo.ShouldDamageInvincible, DamageInfo.CanBeBlocked) == 2)
	{
		return false;
	}
	return false;
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

float UOVDamageComponent::CanbeDamage(bool ShouldDamageInvincible, bool CanbeBlocked)
{
	//0 Block Damage
	//1 Do Damage
	//2 No Damage

	if(!bIsDead && (!bIsInvincible|| ShouldDamageInvincible))
	{
		if(bIsBlocking && CanbeBlocked)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 2;
	}
}

