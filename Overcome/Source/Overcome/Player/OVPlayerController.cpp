// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OVPlayerController.h"
#include "UI/OVHUDWidget.h"

AOVPlayerController::AOVPlayerController()
{
	static ConstructorHelpers::FClassFinder<UOVHUDWidget> OVHUDWidgetRef(TEXT("/Game/UMG/WBP_HUDWidget.WBP_HUDWidget_C"));
	if (OVHUDWidgetRef.Class)
	{
		OVHUDWidgetClass = OVHUDWidgetRef.Class;
	}
}

void AOVPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOblyInputMode;
	SetInputMode(GameOblyInputMode);

	if(IsLocalPlayerController())
	{
		OVHUDWidget = CreateWidget<UOVHUDWidget>(this, OVHUDWidgetClass);
		if (OVHUDWidget)
		{
			OVHUDWidget->AddToViewport();
			OVHUDWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	const FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	SetShowMouseCursor(false);
}

UOVHUDWidget* AOVPlayerController::GetOVHUDWidget() const
{
	return OVHUDWidget;
}
