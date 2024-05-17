// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/OVSkillBase.h"
#include "Character/OVCharacterPlayer.h"

DEFINE_LOG_CATEGORY(LogSkillCharacter);

// Sets default values for this component's properties
UOVSkillBase::UOVSkillBase()
{
	CoolTime = -1;
}


// Called when the game starts
void UOVSkillBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void UOVSkillBase::SkillAction()
{
	Owner=Cast<AOVCharacterPlayer>(GetOwner());
}



