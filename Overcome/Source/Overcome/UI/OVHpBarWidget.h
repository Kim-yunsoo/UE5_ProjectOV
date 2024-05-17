// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OVUserWidget.h"
#include "OVHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVHpBarWidget : public UOVUserWidget
{
	GENERATED_BODY()

public:
	UOVHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual  void NativeConstruct() override;
	
public:
	FORCEINLINE void SetMaxHp(float NewMAxHp){MaxHp = NewMAxHp;}
	void UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();
	
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;
	
	UPROPERTY()
	float CurrentHp;
	
	UPROPERTY()
	float MaxHp;
};
