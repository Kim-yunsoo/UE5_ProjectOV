// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/OVUserWidget.h"
#include "OVTargetWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVTargetWidget : public UOVUserWidget
{
	GENERATED_BODY()
public:
	void UpdateTargetUI(bool IsAim);
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UImage> TargetUIImage;
	
};
