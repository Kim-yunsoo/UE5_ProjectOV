// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVTargetWidget.h"

#include "Components/Image.h"

void UOVTargetWidget::UpdateTargetUI(bool IsAim)
{
	if(IsAim)
		this->SetVisibility(ESlateVisibility::Visible);
	else
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOVTargetWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TargetUIImage = Cast<UImage>(GetWidgetFromName(TEXT("TargetImage")));
	ensure(TargetUIImage);
	this->SetVisibility(ESlateVisibility::Hidden);
}
