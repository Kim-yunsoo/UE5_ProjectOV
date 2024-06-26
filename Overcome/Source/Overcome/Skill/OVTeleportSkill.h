// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/OVSkillBase.h"
#include "OVTeleportSkill.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeleportTimeChangedDelegate, float /*CurrentTime*/);

UCLASS()
class OVERCOME_API UOVTeleportSkill : public UOVSkillBase
{
	GENERATED_BODY()
public:
	UOVTeleportSkill();
	FOnTeleportTimeChangedDelegate OnTeleportTime;

protected:
	
	UPROPERTY()
	float TeleportOffset;
	
	UPROPERTY()
	float TeleportCooltime;
	UPROPERTY()
	float CooldownRemaining;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TObjectPtr<UParticleSystem> Emitter;
	
public:
	virtual void SkillAction() override;
};
