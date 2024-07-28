// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OVUserWidget.h"
#include "OVBossHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVBossHpWidget : public UOVUserWidget
{
	GENERATED_BODY()
	
public:
	UOVBossHpWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void UpdateBossUI(bool bIsVisible);
	UFUNCTION()
	void UpdateHpBar(float NewCurrentHp);
protected:
	virtual  void NativeConstruct() override;

	

	UPROPERTY()
	TObjectPtr<class UProgressBar> BossHpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> BossText;
	
	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
