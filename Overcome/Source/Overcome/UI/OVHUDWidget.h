// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OVHUDWidget.generated.h"

class UOVInteractionWidget;
class UOVMainMenu;
struct FInteractionData;
/**
 * 
 */
UCLASS()
class OVERCOME_API UOVHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UOVHUDWidget(const FObjectInitializer& ObjectInitializer);
	void UpdateTarget(bool bIsShowUI);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UOVTargetWidget> TargetWidget;
public:
	UPROPERTY(EditDefaultsOnly,Category="Widgets")
	TSubclassOf<UOVMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly,Category="Widgets")
	TSubclassOf<UOVInteractionWidget> InteractionWidgetClass;
	
	
	UPROPERTY()
	TObjectPtr<class UOVStatWidget> StatWidget;

	UPROPERTY()
	TObjectPtr<class UOVMainMenu> MainMenuWidget;
	
	UPROPERTY()
	TObjectPtr<class UOVInteractionWidget> InteractionWidget ;

	bool bIsMenuVisible;

	void DisplayMenu();
	void HideMenu();

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractionData* InteractionData);
};
