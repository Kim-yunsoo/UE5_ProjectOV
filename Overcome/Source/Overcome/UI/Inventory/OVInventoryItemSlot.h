// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OVInventoryItemSlot.generated.h"

class UTextBlock;
class UImage;
class UBorder;
class UOVInventoryTooltip;
class UOVDragItemVisual;
class UOVItemBase;
/**
 * 
 */
UCLASS()
class OVERCOME_API UOVInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetItemReference(UOVItemBase* ItemIn){ItemReference = ItemIn ; };
	FORCEINLINE UOVItemBase* GetItemReference() const {return ItemReference;}
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UOVDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UOVInventoryTooltip> ToolTipClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	TObjectPtr<UOVItemBase> ItemReference;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot",meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot",meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
};
