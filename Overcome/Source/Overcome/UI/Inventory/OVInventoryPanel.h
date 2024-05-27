// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OVInventoryPanel.generated.h"

class UOVInventoryItemSlot;
class UOVInventoryComponent;
class AOVCharacterPlayer;
class UTextBlock;
class UWrapBox;
/**
 * 
 */
UCLASS()
class OVERCOME_API UOVInventoryPanel : public UUserWidget
{
	GENERATED_BODY()

	TObjectPtr<UWrapBox> InventoryPanel;
	TObjectPtr<UTextBlock> CapacityInfo;
	TObjectPtr<AOVCharacterPlayer> PlayerCharacter;
	TObjectPtr<UOVInventoryComponent> InventoryReference;

	TSubclassOf<UOVInventoryItemSlot> InventorySlotClass;
	
};
