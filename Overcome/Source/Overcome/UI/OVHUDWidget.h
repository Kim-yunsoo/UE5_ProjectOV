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

	UFUNCTION()
	void UpdateBossUI(bool bIsShowUI);
public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UOVTargetWidget> TargetWidget;
public:
	UPROPERTY(EditDefaultsOnly,Category="Widgets")
	TSubclassOf<class UOVMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly,Category="Widgets")
	TSubclassOf<class UOVInteractionWidget> InteractionWidgetClass;
	
	UPROPERTY()
	TObjectPtr<class UOVStatWidget> TeleportSkillWidget;

	UPROPERTY()
	TObjectPtr<class UOVStatWidget> ShieldSkillWidget;
	
	UPROPERTY()
	TObjectPtr<class UOVStatWidget> StatWidget;

	UPROPERTY()
	TObjectPtr<class UOVBossHpWidget> BossHpWidget;
	TObjectPtr<UOVMainMenu> MainMenuWidget;
	
	UPROPERTY()
	TObjectPtr<UOVInteractionWidget> InteractionWidget ;

	bool bIsMenuVisible;

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractionData) const;
	void UpdateTeleportTime(float NewCurrentTime);
	void UpdateShieldTime(float NewCurrentTime);
};
