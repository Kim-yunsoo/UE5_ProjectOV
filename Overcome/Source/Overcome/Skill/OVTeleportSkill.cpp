// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/OVTeleportSkill.h"
#include "Character/OVCharacterPlayer.h"


UOVTeleportSkill::UOVTeleportSkill()
{
	PrimaryComponentTick.bCanEverTick = true;
	//CoolTime = 10;
	TeleportOffset = 600.0f;
	TeleportCooltime = 5.f;
	CooldownRemaining = 5.f;
}

void UOVTeleportSkill::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Owner)
	{
		return;
	}
	float CooldownPercent = 0.f;
	
	if (Owner->bIsActiveTeleportSkill) 
	{
		OnTeleportTime.Broadcast(CooldownPercent);
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
		CooldownPercent = CooldownRemaining / TeleportCooltime;
		
		OnTeleportTime.Broadcast(CooldownPercent);
		//UpdateCooldownBar();
	}
}

void UOVTeleportSkill::SkillAction()
{
	UE_LOG(LogSkillCharacter, Log, TEXT("StartTeleportSkill"));
	Super::SkillAction();
	FVector TargetLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * TeleportOffset;
	GetOwner()->TeleportTo(TargetLocation, GetOwner()->GetActorRotation(), false, true);
	
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		{
			Owner->bIsActiveTeleportSkill = true;
			CooldownRemaining = 5;
			UE_LOG(LogSkillCharacter, Log, TEXT("EndTeleportSkill"));
		}
	), TeleportCooltime, false, -1.0f);
}
