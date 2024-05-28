// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVBossHpWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/OVCharateWidgetInterface.h"

UOVBossHpWidget::UOVBossHpWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = 500;
}

void UOVBossHpWidget::UpdateBossUI(bool bIsVisible)
{
	if(bIsVisible)
		this->SetVisibility(ESlateVisibility::Visible);
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("HELLO"));
		this->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOVBossHpWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BossHpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar")));

	BossText = Cast<UTextBlock>(GetWidgetFromName(TEXT("BossName")));
}

void UOVBossHpWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if(BossHpProgressBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss uypdateHpbar %f"),  NewCurrentHp);
		BossHpProgressBar->SetPercent(NewCurrentHp/MaxHp);
	}
	CurrentHp = NewCurrentHp;
}
