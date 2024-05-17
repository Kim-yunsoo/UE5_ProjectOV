// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OVCharacterBase.h"
#include "OVCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API AOVCharacterNonPlayer : public AOVCharacterBase
{
	GENERATED_BODY()
public:
	AOVCharacterNonPlayer();
	
protected:
	float DeadEventDelayTime = 3.0f;
	
	virtual void SetDead() override;
};
