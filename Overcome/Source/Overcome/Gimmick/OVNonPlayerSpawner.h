// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AI/OVCharacterNonPlayer.h"
#include "GameFramework/Actor.h"
#include "OVNonPlayerSpawner.generated.h"

UCLASS()
class OVERCOME_API AOVNonPlayerSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AOVNonPlayerSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	TObjectPtr<class UBoxComponent> Trigger;
	
	UPROPERTY(EditAnywhere, Category = NonPlayer)
	TSubclassOf<class AOVCharacterNonPlayer> NonPlayer;

};
