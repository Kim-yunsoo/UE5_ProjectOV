// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UOVStartUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UUOVStartUI::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Play = Cast<UButton>(GetWidgetFromName(TEXT("Button_Play")));
	Button_Quit = Cast<UButton>(GetWidgetFromName(TEXT("Button_Quit")));
	Button_Play->OnClicked.AddDynamic(this, &UUOVStartUI::Button_PlayCallback);
	Button_Quit->OnClicked.AddDynamic(this, &UUOVStartUI::Button_QuitCallback);
	const FInputModeGameAndUI InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);
}

void UUOVStartUI::Button_PlayCallback()
{
	SetVisibility(ESlateVisibility::Hidden);
	const FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);
}

void UUOVStartUI::Button_QuitCallback()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);

}
