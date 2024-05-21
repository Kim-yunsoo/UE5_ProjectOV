// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OVCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttackChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnStatChangedDelegate, float /*Hp*/, float /*Mp*/, float /*Attack*/)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOME_API UOVCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOVCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpchanged;
	FOnAttackChangedDelegate OnAttackChanged;
	FOnStatChangedDelegate OnStatChanged;

	FORCEINLINE float GetMaxHp() {return MaxHp;}
	FORCEINLINE void SetMaxHp(float NewHp) {MaxHp = NewHp;}
	FORCEINLINE float GetCurrentHp() {return CurrentHp;}
	FORCEINLINE float GetCurrentMp() {return CurrentMp;}
	FORCEINLINE float GetCurrentAttack() {return CurrentAttack;}
	float ApplyDamage(float InDamage);
	

	void SetHp(float NewHp);
	void SetMp(float NewMp);
	void SetAttack(float NewAttack);
	
protected:	
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp; // 일단 100으로

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentMp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentAttack;
		
};
