// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVBatteryWidget.h"

#include "Components/TextBlock.h"
#include "Game/OVGameMode.h"
#include "Game/OVGameState.h"

UOVBatteryWidget::UOVBatteryWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	Battery = 0;
}

FString UOVBatteryWidget::PrintBatteryCount()
{
	return FString::Printf(TEXT("0%d"), Battery);
}

void UOVBatteryWidget::UpdateBatteryCount(int32 count)
{
	Battery = count;
	UE_LOG(LogTemp,Warning, TEXT("Battery %d"), Battery);
	BatteryCount->SetText(FText::FromString((PrintBatteryCount())));
}

void UOVBatteryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BatteryCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Count")));
}

void UOVBatteryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	AOVGameMode* GameMode = Cast<AOVGameMode>(GetWorld()->GetAuthGameMode());
	if(GameMode->Battery == GameMode->GoalCount)
	{
		FTimerHandle VisibilityTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(VisibilityTimerHandle, [this]()
		{
			SetVisibility(ESlateVisibility::Collapsed);
		}, 2.0f, false);
	}
}
