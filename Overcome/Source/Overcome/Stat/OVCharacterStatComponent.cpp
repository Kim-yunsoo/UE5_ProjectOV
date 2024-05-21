// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/OVCharacterStatComponent.h"

// Sets default values for this component's properties
UOVCharacterStatComponent::UOVCharacterStatComponent()
{
	//MaxHp = 100;
	CurrentHp = MaxHp;
	CurrentAttack = 10;
	CurrentMp = 100;
}


// Called when the game starts
void UOVCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	//MaxHp = 100;
	SetHp(MaxHp);
	
}

float UOVCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
	SetHp(PrevHp - ActualDamage);
	if(CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UOVCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	OnHpchanged.Broadcast(CurrentHp);
	OnStatChanged.Broadcast(CurrentHp, CurrentMp, CurrentAttack);
}

void UOVCharacterStatComponent::SetMp(float NewMp)
{
	CurrentMp = FMath::Clamp<float>(NewMp, 0.0f, 100);
	OnStatChanged.Broadcast(CurrentHp, CurrentMp, CurrentAttack);
}

void UOVCharacterStatComponent::SetAttack(float NewAttack)
{
	CurrentAttack = FMath::Clamp<float>(NewAttack, 0.0f, 100);
	OnAttackChanged.Broadcast(CurrentAttack);
	OnStatChanged.Broadcast(CurrentHp, CurrentMp, CurrentAttack);
}


