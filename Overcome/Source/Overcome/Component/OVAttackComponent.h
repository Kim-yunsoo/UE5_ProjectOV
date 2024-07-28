// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OVAttackComponent.generated.h"


class UNiagaraSystem;
struct FDamageInfo;

DECLARE_MULTICAST_DELEGATE(FOnAttackEndedDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOME_API UOVAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOVAttackComponent();

	FOnAttackEndedDelegate OnAttackEnded;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void FireBullet(FVector Start, FVector End, FDamageInfo DamageInfo);

	UFUNCTION()
	void AttackSlash(float Radius, float Length, FDamageInfo DamageInfo);

	UFUNCTION()
	void AttackAOESlash(float Radius, FDamageInfo DamageInfo);

	UFUNCTION()
	void AttackAOEStrike(FDamageInfo DamageInfo);

	UFUNCTION()
	void JumpTarget(AActor* AttackTarget);

	UFUNCTION()
	FVector CalculateFutureActorLocation(AActor* AttackTarget, float Timer);

	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);
	
	UPROPERTY()
	TObjectPtr<UNiagaraSystem> EmitterHit;

	UPROPERTY()
	TObjectPtr<UNiagaraSystem> BossHit;
	
};
