// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OVInventoryTooltip.generated.h"

class UTextBlock;
class UOVInventoryItemSlot;
/**
 * 
 */
UCLASS()
class OVERCOME_API UOVInventoryTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UOVInventoryItemSlot> InventorySlotBeginHovered;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemType;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> UsageText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemDescription;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MaxStackSize;

protected:
	virtual void NativeConstruct() override;
	
};
