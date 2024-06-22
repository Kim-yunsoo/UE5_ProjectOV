// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OVGameMode.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBatteryCountDelegate, int /*CurrentTime*/);

UCLASS()
class OVERCOME_API AOVGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public: 	
	AOVGameMode();
	void SetBatteryCount(int NewBattery);
	
	FOnBatteryCountDelegate OnBatteryCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Battery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GoalCount;

	UFUNCTION()
	void ChangeLevel();
	
	UFUNCTION()
	void UnloadOldLevel();
	
	UPROPERTY()
	float timestamp;

	
};
