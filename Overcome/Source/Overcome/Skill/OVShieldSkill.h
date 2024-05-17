// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/OVSkillBase.h"
#include "OVShieldSkill.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVShieldSkill : public UOVSkillBase
{
	GENERATED_BODY()
public:	
	UOVShieldSkill();
	
protected:
	
	UPROPERTY()
	float ShieldCooltime;
	
public:
	virtual void SkillAction() override;
};
