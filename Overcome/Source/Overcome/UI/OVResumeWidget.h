// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OVResumeWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVResumeWidget : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void ClickResume();

};
