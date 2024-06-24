// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVResumeWidget.h"
#include "Player/OVPlayerController.h"

void UOVResumeWidget::ClickResume()
{
	AOVPlayerController* Controller = Cast<AOVPlayerController>(GetOwningPlayer());
	Controller->SetPause(false);
	{
		SetVisibility(ESlateVisibility::Collapsed);
		const FInputModeGameOnly InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(false);
	}
}


