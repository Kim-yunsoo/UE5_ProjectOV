// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/OVDamagableInterface.h"
#include "OVDamageComponent.generated.h"


struct FDamageInfo;

DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBlockedDelegate, bool /*Blocked*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageResponseDelegate, E_DamageResponses /*DamageResponse*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOME_API UOVDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOVDamageComponent();

	FOnDeathDelegate OnDeath;
	FOnBlockedDelegate OnBlocked;
	FOnDamageResponseDelegate OnDamageResponse;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

public:
	UPROPERTY()
	float Health; //고민중

	UPROPERTY()
	float MaxHealth; //고민중

	UPROPERTY()
	uint8 bIsInvincible :1;

	UPROPERTY()
	uint8 bIsDead : 1;

	UPROPERTY()
	uint8 bIsInterruptible : 1;

	UPROPERTY()
	uint8 bIsBlocking : 1;

	UPROPERTY()
	uint8 bIsShieldSkill :1;

	UFUNCTION()
	bool TakeDamage(FDamageInfo DamageInfo);

	UFUNCTION()
	float Heal(float Amount);

	UFUNCTION()
	void SetMaxHealth(float NewMaxHealth);

	UFUNCTION()
	float CanbeDamage(bool ShouldDamageInvincible, bool CanbeBlocked);
};
