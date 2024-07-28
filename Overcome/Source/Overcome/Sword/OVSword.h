// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OVSword.generated.h"

UCLASS()
class OVERCOME_API AOVSword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOVSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh;
};
