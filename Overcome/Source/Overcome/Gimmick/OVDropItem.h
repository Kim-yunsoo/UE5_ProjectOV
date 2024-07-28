// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OVDropItem.generated.h"

class UBoxComponent;

UCLASS()
class OVERCOME_API AOVDropItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AOVDropItem();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, Category = "Trigger")
	TObjectPtr<UBoxComponent> Trigger;


	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	int32 Battery;
};
