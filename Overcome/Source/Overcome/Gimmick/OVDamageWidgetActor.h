// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OVDamageWidgetActor.generated.h"

UCLASS()
class OVERCOME_API AOVDamageWidgetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOVDamageWidgetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> DamageComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDamage(const float &InDamage);
};
