// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVStatWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UOVStatWidget::UOVStatWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UOVStatWidget::UpdateStatWidget(float NewCurrentHp, float NewCurrentMp, float NewCurrentAttack)
{
	if(HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp/100);
	}
	if(MpProgressBar)
	{
		MpProgressBar->SetPercent(NewCurrentMp/100);
	}
	CurrentAttack = NewCurrentAttack;
	AttackStat->SetText(FText::FromString(GetAttackStatText()));

	TeleportBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBTeleport")));
	ensure(TeleportBar);
	ShieldBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBShield")));
	ensure(ShieldBar);

	if (TeleportBar)
	{
		TeleportBar->SetPercent(0.0f);
	}

	if (ShieldBar)
	{
		ShieldBar->SetPercent(0.0f);
	}
}

void UOVStatWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	MpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbMpBar")));
	AttackStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtAttackStat")));
}

FString UOVStatWidget::GetAttackStatText()
{
	return FString::Printf(TEXT("Attack : %0.f"), CurrentAttack);
}

void UOVStatWidget::UpdateTeleportBar(float NewCurrentPercent)
{
	TeleportBar->SetPercent(NewCurrentPercent);
}

void UOVStatWidget::UpdateShieldBar(float NewCurrentPercent)
{
	ShieldBar->SetPercent(NewCurrentPercent);
}
