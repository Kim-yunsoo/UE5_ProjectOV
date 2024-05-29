// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OVEnemy_Base.h"
#include "Interface/OVDamagableInterface.h"
#include "Sword/OVSword.h"
#include "OVAIBoss.generated.h"

class AOVAIBossController;
class UOVHUDWidget;
/**
 * 
 */

UCLASS()
class OVERCOME_API AOVAIBoss : public AOVCharacterBase, public IOVEnemyAIInterface, public IOVDamagableInterface
{
	GENERATED_BODY()
public:
	AOVAIBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FAIEnemyAttackFinished OnDefaultAttackFinished;

	UPROPERTY()
	AOVAIBossController* BossController;

protected:
	
	
	virtual void SetAIDefaultAttackDelegate(const FAIEnemyAttackFinished& InOnAttackFinished) override;
	virtual void DefaultAttack() override;
	

	//Sword
	UPROPERTY()
	AOVSword* Sword;

	UPROPERTY()
	AOVSword* Sword_l;
	
	UFUNCTION()
	virtual void EauipWeapon() override;
	
	UFUNCTION()
	virtual void UneauipWeapon() override;

	virtual float GetIsWieldingWeapon() override;

	virtual  void SetMovementSpeed(E_MovementSpeed SpeedValue) override;

	UPROPERTY()
	uint8 bIsFirst : 1;
	
	UPROPERTY()
	uint8 bIsEquipSword : 1;

	UPROPERTY()
	uint8 bIsWieldingWeapon : 1;

	E_MovementSpeed MovementSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sword")
	TSubclassOf<AOVSword> SwordClass;
	
	//Montage
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> StaggerMontage;
	
	UFUNCTION()
	void OnDefaultAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnStaggerMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UPROPERTY(EditAnywhere)
	E_AIState AIState;


public:
	UFUNCTION()
	virtual void SetState(E_AIState AIStateValue) override;

	UFUNCTION()
	virtual E_AIState GetState() override;


	virtual void PostInitializeComponents() override;

	//Damage
	virtual float GetCurrentHealth();
	virtual float GetMaxHealth();
	virtual float Heal(float Amount); 
	virtual bool TakeDamage(FDamageInfo DamageInfo);
	virtual bool IsDead() override;

	void Blocked(bool CanBeParried);
	void DamageResponse(E_DamageResponses DamageResponses);
	virtual void SetDead() override;

	
public:
	void SlashCheck();
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UOVDamageComponent> DamageComponent;



	
	//TEST ATTACK
	UFUNCTION()
	void TestAttack();

	
	
};
