// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OVHUDWidget.generated.h"

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
	UPROPERTY()
	TObjectPtr<class UOVStatWidget> StatWidget;
	
};
