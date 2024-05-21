// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/OVCharateWidgetInterface.h"

UOVHpBarWidget::UOVHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
	CurrentHp = MaxHp;
}

void UOVHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));


	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	
	IOVCharateWidgetInterface* CharacterWidget = Cast<IOVCharateWidgetInterface>(OwningActor);
	if(CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UOVHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if(HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp/MaxHp);
	}
	CurrentHp = NewCurrentHp;
	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

FString UOVHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp);
}
