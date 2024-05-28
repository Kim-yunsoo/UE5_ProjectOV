// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OVMainMenu.generated.h"

class AOVCharacterPlayer;
/**
 * 
 */
UCLASS()
class OVERCOME_API UOVMainMenu : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY()
	AOVCharacterPlayer* PlayerCharacter;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

};
