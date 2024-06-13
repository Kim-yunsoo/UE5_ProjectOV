// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVMainMenu.h"
#include "Character/OVCharacterPlayer.h"
#include "Inventory/OVItemDragDropOperation.h"
#include "Item/OVItemBase.h"

void UOVMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UOVMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerCharacter = Cast<AOVCharacterPlayer>(GetOwningPlayerPawn());
	SetKeyboardFocus(); //키보드
	SetIsFocusable(true);
}

bool UOVMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation) //Main보다 먼저 감지하는듯한 ...
{
	//return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	const UOVItemDragDropOperation* ItemDragDrop = Cast<UOVItemDragDropOperation>(InOperation);

	if(PlayerCharacter && ItemDragDrop->SourceItem)
	{
		PlayerCharacter->DropItem(ItemDragDrop->SourceItem,ItemDragDrop->SourceItem->Quantity);
		return true;
	}
	return false;
}

// void UOVMainMenu::CloseMainMenu()
// {
// 	SetVisibility(ESlateVisibility::Collapsed);
// 	const FInputModeGameOnly InputMode;
// 	GetOwningPlayer()->SetInputMode(InputMode);
// 	GetOwningPlayer()->SetShowMouseCursor(false);
// }
