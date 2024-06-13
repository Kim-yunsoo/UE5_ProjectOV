// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/OVGunSkill.h"

#include "Character/OVCharacterPlayer.h"

UOVGunSkill::UOVGunSkill()
{
	PrimaryComponentTick.bCanEverTick = true;
	GunCooltime = 5.f;
	CooldownRemaining = 5.f;
}

void UOVGunSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Owner)
	{
		return;
	}
	float CooldownPercent = 0.f;
	
	if (Owner->bIsActiveGunSkill) 
	{
		OnGunTime.Broadcast(CooldownPercent);
		return;
	}
	
	if (CooldownRemaining > 0)
	{
		CooldownRemaining -= DeltaTime;
		if (CooldownRemaining < 0)
		{
			CooldownRemaining = 0;
		}
		// 위젯 업데이트
		CooldownPercent = CooldownRemaining / GunCooltime;
		
		OnGunTime.Broadcast(CooldownPercent);
		//UpdateCooldownBar();
	}
}

void UOVGunSkill::SkillAction()
{
	UE_LOG(LogSkillCharacter, Log, TEXT("StartGunSkill"));
	Super::SkillAction();
	
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		{
			Owner->bIsActiveGunSkill = true;
			CooldownRemaining = 5;
			UE_LOG(LogSkillCharacter, Log, TEXT("EndGunSkill"));
		}
	), GunCooltime, false, -1.0f);
}
