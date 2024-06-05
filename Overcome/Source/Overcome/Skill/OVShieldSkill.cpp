// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/OVShieldSkill.h"
#include "Character/OVCharacterPlayer.h"
#include "Component/OVDamageComponent.h"

UOVShieldSkill::UOVShieldSkill()
{
	PrimaryComponentTick.bCanEverTick = true;
	ShieldCooltime = 5.f;
	CooldownRemaining = 5.f;
}

void UOVShieldSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Owner)
	{
		return;
	}
	float CooldownPercent = 0.f;
	
	if (Owner->bIsActiveShieldSkill) 
	{
		OnShieldTime.Broadcast(CooldownPercent);
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
		CooldownPercent = CooldownRemaining / ShieldCooltime;
		UE_LOG(LogTemp,	Warning, TEXT("Cooldown %f"), CooldownPercent);
		UE_LOG(LogTemp,	Warning, TEXT("CooldownRemaining %f"), CooldownRemaining);
		OnShieldTime.Broadcast(CooldownPercent);
		//UpdateCooldownBar();
	}
}

void UOVShieldSkill::SkillAction()
{
	UE_LOG(LogSkillCharacter, Log, TEXT("StartShieldSkill"));
	Super::SkillAction();
	
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		{
			Owner->bIsActiveShieldSkill = true;
			CooldownRemaining = 5;
			Owner->DamageComponent->bIsShieldSkill = false;
			UE_LOG(LogSkillCharacter, Log, TEXT("EndShieldSkill"));
		}
	), ShieldCooltime, false, -1.0f);
}
