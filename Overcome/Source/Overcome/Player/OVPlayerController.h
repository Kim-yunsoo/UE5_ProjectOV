// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OVPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API AOVPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AOVPlayerController();
	
	protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UOVHUDWidget> OVHUDWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UOVHUDWidget> OVHUDWidget;
};
