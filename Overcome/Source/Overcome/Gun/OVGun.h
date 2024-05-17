// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "OVGun.generated.h"

UCLASS()
class OVERCOME_API AOVGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOVGun();

	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FORCEINLINE int32 GetBulletCount(){return BulletCount;};
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere);
	float MaxRange = 1000.f;

	UPROPERTY(EditAnywhere);
	float Damage;	

	UPROPERTY(EditAnywhere)
	int32 BulletCount;
	
	UPROPERTY()
	TObjectPtr<UNiagaraSystem> EmitterHit;
};
