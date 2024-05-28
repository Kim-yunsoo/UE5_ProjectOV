// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVHUDWidget.h"

#include "OVBossHpWidget.h"
#include "OVStatWidget.h"
#include "OVTargetWidget.h"
#include "Components/VerticalBox.h"
#include "Interface/OVCharacterHUDInterface.h"

UOVHUDWidget::UOVHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{       
}

void UOVHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TargetWidget = Cast<UOVTargetWidget>(GetWidgetFromName(TEXT("WBP_TargetWidget")));
	StatWidget = Cast<UOVStatWidget>(GetWidgetFromName(TEXT("WBP_Stat")));
	BossHpWidget = Cast<UOVBossHpWidget>(GetWidgetFromName(TEXT("WBP_BossHpBar")));

	IOVCharacterHUDInterface* CharacterWidget = Cast<IOVCharacterHUDInterface>(GetOwningPlayerPawn());
	if(CharacterWidget)
	{
		CharacterWidget->SetupHUDWidget(this);
	}
}

void UOVHUDWidget::UpdateTarget(bool bIsShowUI)
{
	TargetWidget->UpdateTargetUI(bIsShowUI);
}

void UOVHUDWidget::UpdateBossUI(bool bIsShowUI)
{
	BossHpWidget->UpdateBossUI(bIsShowUI);
}
