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
	
	protected:
	virtual void BeginPlay() override;
};
