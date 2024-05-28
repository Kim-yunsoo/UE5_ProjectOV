// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OVDragItemVisual.generated.h"

class UBorder;
class UImage;
class UTextBlock;


UCLASS()
class OVERCOME_API UOVDragItemVisual : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual",meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;
	
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual",meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;
	
};
