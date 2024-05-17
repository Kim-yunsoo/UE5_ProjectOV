// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVWidgetComponent.h"
#include "OVUserWidget.h"

void UOVWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UOVUserWidget* OVUserWidget = Cast<UOVUserWidget>(GetWidget());
	if(OVUserWidget)
	{
		OVUserWidget->SetOwningActor(GetOwner());
	}
}
