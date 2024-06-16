// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVResumeWidget.h"

#include "Game/OVGameState.h"
#include "Kismet/GameplayStatics.h"

void UOVResumeWidget::ClickResume()
{
	AOVGameState* GameState = Cast<AOVGameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->bIsResumeMenuVisible = false;
	{
		SetVisibility(ESlateVisibility::Collapsed);
		const FInputModeGameOnly InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(false);
	}
}
