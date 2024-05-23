// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OVEnemy_Base.h"
#include "Sword/OVSword.h"
#include "OVAIBoss.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API AOVAIBoss : public AOVEnemy_Base
{
	GENERATED_BODY()
public:
	AOVAIBoss();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	AOVSword* Sword;

	UPROPERTY()
	AOVSword* Sword_l;

	UPROPERTY()
	uint8 bIsEquipSword : 1;

	UPROPERTY()
	uint8 bIsWieldingWeapon : 1;
	

	UFUNCTION()
	void EauipWeapon();
	
	UFUNCTION()
	void UneauipWeapon();
	
	UPROPERTY(EditDefaultsOnly, Category = "Sword")
	TSubclassOf<AOVSword> SwordClass;
};
