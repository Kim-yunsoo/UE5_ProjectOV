// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OVBatteryWidget.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBatteryCountDelegate, int /*CurrentTime*/);

UCLASS()
class OVERCOME_API UOVBatteryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UOVBatteryWidget(const FObjectInitializer& ObjectInitializer);
	FString PrintBatteryCount();
	void UpdateBatteryCount(int count);
	
protected:
	virtual  void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
	int32 Battery;

	UPROPERTY()
	TObjectPtr<class UTextBlock> BatteryCount;
};
