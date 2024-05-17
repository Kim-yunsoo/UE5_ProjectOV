// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/OVUserWidget.h"
#include "OVStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVStatWidget : public UOVUserWidget
{
	GENERATED_BODY()

public:
	UOVStatWidget(const FObjectInitializer& ObjectInitializer);
	void UpdateStatWidget(float NewCurrentHp, float NewCurrentMp, float NewCurrentAttack);
	FString GetAttackStatText();
protected:
	virtual  void NativeConstruct() override;
	
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UProgressBar> MpProgressBar;
	
	UPROPERTY()
	TObjectPtr<class UTextBlock> AttackStat;

	UPROPERTY()
	float CurrentAttack;
};
