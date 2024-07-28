// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OVEnemy_Base.h"
#include "Character/OVCharacterBase.h"
#include "Interface/OVDamagableInterface.h"
#include "Interface/OVEnemyAIInterface.h"
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
	FAIEnemyTeleportFinished OnTeleportFinished;
	
	UPROPERTY()
	AOVAIBossController* BossController;

protected:
	
	
	virtual void SetAIDefaultAttackDelegate(const FAIEnemyAttackFinished& InOnAttackFinished) override;
	virtual void DefaultAttack() override;
	virtual void BossAttack(E_BossAttack BossAttack) override;

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

	virtual  void SetMovementSpeed(EMovementSpeed SpeedValue) override;

	UPROPERTY()
	uint8 bIsFirst : 1;
	
	UPROPERTY()
	uint8 bIsEquipSword : 1;

	UPROPERTY()
	uint8 bIsWieldingWeapon : 1;

	EMovementSpeed MovementSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sword")
	TSubclassOf<AOVSword> SwordClass;
	
	//Montage
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> StaggerMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> DeathMontage;
	
	UFUNCTION()
	void OnDefaultAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnStaggerMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UPROPERTY(EditAnywhere)
	EAIState AIState;

public:
	UFUNCTION()
	virtual void SetState(EAIState AIStateValue) override;

	UFUNCTION()
	virtual EAIState GetState() override;
	virtual void PostInitializeComponents() override;

	//Damage
	virtual float GetCurrentHealth() override;
	virtual float GetMaxHealth() override;
	virtual float Heal(float Amount) override ; 
	virtual bool TakeDamage(FDamageInfo DamageInfo) override ;
	virtual bool IsDead() override;
	virtual bool IsAttacking() override;
	virtual void SetIsInterruptible(bool bIsInterruptibleValue) override;
	virtual void BossTeleport(FVector Location) override;
	virtual void SetAITeleportDelegate(const FAIEnemyTeleportFinished& InOnTeleportFinished) override;
	virtual void SetIsInvincible(bool bIsInvincibleValue)override;

	void BossTeleportEnd();
	void Blocked(bool CanBeParried);
	void DamageResponse(E_DamageResponses DamageResponses);
	virtual void SetDead() override;
	UPROPERTY()
	uint8 bIsAttacking : 1;

	UPROPERTY()
	uint8 bIsTeleporting : 1;
	
public:
	void SlashCheck();
	void AOESlashCheck();
	void JumpCheck();
	void SmashCheck();
	void StrikeCheck();

	UFUNCTION()
	void AOEDelegate(AActor* OverlappedActor);

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UOVDamageComponent> DamageComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UOVAttackComponent> AttackComponent;
	
	//Attack
	UFUNCTION()
	void AttackCombo1();

	UFUNCTION()
	void AttackCombo2();
	
	UFUNCTION()
	void AttackJump();

	UFUNCTION()
	void AttackQuick();

	UFUNCTION()
	void AttackGroundSmash();
	
	UFUNCTION()
	void AttackAOE();
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> AttackCombo1Montage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> AttackCombo2Montage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> AttackJumpMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> AttackQuickMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> AttackGroundSmashMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> AttackAOEMontage;
	
	//TEST ATTACK
	UFUNCTION()
	void TestAttack();
};
