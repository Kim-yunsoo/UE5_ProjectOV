// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "OVItemDragDropOperation.generated.h"

class UOVInventoryComponent;
class UOVItemBase;
/**
 * 
 */
UCLASS()
class OVERCOME_API UOVItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	TObjectPtr<UOVItemBase> SourceItem;

	UPROPERTY()
	TObjectPtr<UOVInventoryComponent> SourceInventory;
	
};
