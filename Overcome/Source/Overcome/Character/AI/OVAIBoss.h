// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OVCharacterBase.h"
#include "Sword/OVSword.h"
#include "OVAIBoss.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API AOVAIBoss : public AOVCharacterBase
{
	GENERATED_BODY()
public:
	AOVAIBoss();


protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	AOVSword* Sword;

	
	UPROPERTY(EditDefaultsOnly, Category = "Sword")
	TSubclassOf<AOVSword> SwordClass;
};
