// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/OVSkillBase.h"
#include "OVGunSkill.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGunTimeChangedDelegate, float /*CurrentTime*/);


UCLASS()
class OVERCOME_API UOVGunSkill : public UOVSkillBase
{
	GENERATED_BODY()
public:
	UOVGunSkill();
	FOnGunTimeChangedDelegate OnGunTime;

	UPROPERTY()
	float GunCooltime;
	UPROPERTY()
	float CooldownRemaining;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	virtual void SkillAction() override;
};
