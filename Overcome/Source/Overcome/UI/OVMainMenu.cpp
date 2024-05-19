// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVMainMenu.h"

#include "Character/OVCharacterPlayer.h"

void UOVMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UOVMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AOVCharacterPlayer>(GetOwningPlayer());
}

bool UOVMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	//cast operation to item drag drop, ensure player is valid, call drop item on player
	

	
}
