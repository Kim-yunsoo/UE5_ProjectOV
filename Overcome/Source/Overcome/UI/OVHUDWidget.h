// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OVHUDWidget.generated.h"

struct FInteractableData;
/**
 * 
 */
UCLASS()
class OVERCOME_API UOVHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UOVHUDWidget(const FObjectInitializer& ObjectInitializer);
	void UpdateTarget(bool bIsShowUI) const;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UOVTargetWidget> TargetWidget;
public:
	UPROPERTY(EditDefaultsOnly,Category="Widgets")
	TSubclassOf<class UOVMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly,Category="Widgets")
	TSubclassOf<class UOVInteractionWidget> InteractionWidgetClass;
	
	
	UPROPERTY()
	TObjectPtr<class UOVStatWidget> StatWidget;

	UPROPERTY()
	TObjectPtr<UOVMainMenu> MainMenuWidget;
	
	UPROPERTY()
	TObjectPtr<UOVInteractionWidget> InteractionWidget ;

	bool bIsMenuVisible;

	void DisplayMenu();
	void HideMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractionData) const;
};
