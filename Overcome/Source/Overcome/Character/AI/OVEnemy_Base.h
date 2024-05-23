// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OVCharacterBase.h"
#include "OVEnemy_Base.generated.h"

UCLASS()
class OVERCOME_API AOVEnemy_Base : public AOVCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOVEnemy_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
