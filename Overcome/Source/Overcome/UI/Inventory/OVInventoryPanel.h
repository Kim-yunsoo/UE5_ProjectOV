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

	UFUNCTION()
	void RefreshInventory();
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWrapBox> InventoryPanel;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CapacityInfo;
	UPROPERTY()
	TObjectPtr<AOVCharacterPlayer> PlayerCharacter;
	UPROPERTY()
	TObjectPtr<UOVInventoryComponent> InventoryReference;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOVInventoryItemSlot> InventorySlotClass;

protected:
	void SetInfoText() const ;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
};
