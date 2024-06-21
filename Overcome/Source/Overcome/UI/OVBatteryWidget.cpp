// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVBatteryWidget.h"

#include "Components/TextBlock.h"

UOVBatteryWidget::UOVBatteryWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	Battery = 3;
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
