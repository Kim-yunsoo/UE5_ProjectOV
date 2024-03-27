// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OVPlayerController.h"

void AOVPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOblyInputMode;
	SetInputMode(GameOblyInputMode);
}
