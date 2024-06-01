// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OVAOE_LightningStrike.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverlapActor, AActor*, OverlappedActor);

UCLASS()
class OVERCOME_API AOVAOE_LightningStrike : public AActor
{
	GENERATED_BODY()
	
public:	
	AOVAOE_LightningStrike();

	UFUNCTION()
	void Trigger();

	UPROPERTY(BlueprintAssignable)
	FOnOverlapActor OnOverlapActor;
	
	UPROPERTY(EditAnywhere)
	UDecalComponent* DecalComponent;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInterface> DecalMaterial;
public:	
	virtual void Tick(float DeltaTime) override;

	
};
