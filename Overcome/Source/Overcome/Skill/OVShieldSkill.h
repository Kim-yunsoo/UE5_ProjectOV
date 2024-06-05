// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/OVSkillBase.h"
#include "OVShieldSkill.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnShieldTimeChangedDelegate, float /*CurrentTime*/);

UCLASS()
class OVERCOME_API UOVShieldSkill : public UOVSkillBase
{
	GENERATED_BODY()
public:	
	UOVShieldSkill();
	FOnShieldTimeChangedDelegate OnShieldTime;
protected:
	UPROPERTY()
	float ShieldCooltime;
	
	UPROPERTY()
	float CooldownRemaining;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void SkillAction() override;
};
