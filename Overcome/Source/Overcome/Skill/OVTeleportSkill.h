// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/OVSkillBase.h"
#include "OVTeleportSkill.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVTeleportSkill : public UOVSkillBase
{
	GENERATED_BODY()
public:
	UOVTeleportSkill();
	
protected:

	UPROPERTY()
	float TeleportOffset;
	
	UPROPERTY()
	float TeleportCooltime;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	virtual void SkillAction() override;
};
