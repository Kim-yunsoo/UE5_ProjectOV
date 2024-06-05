// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UOVStartUI.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UUOVStartUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* Button_Play;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* Button_Quit;

	UFUNCTION()
	void Button_PlayCallback();

	UFUNCTION()
	void Button_QuitCallback();
};
