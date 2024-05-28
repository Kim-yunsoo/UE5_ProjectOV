// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OVCharacterBase.h"
#include "Interface/OVEnemyAIInterface.h"
#include "OVEnemy_Base.generated.h"


class UOVHUDWidget;
class AOVSword;


UCLASS()
class OVERCOME_API AOVEnemy_Base : public AOVCharacterBase, public IOVEnemyAIInterface
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

protected:
	FAIEnemyAttackFinished OnDefaultAttackFinished;
	
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
	uint8 bIsEquipSword : 1;

	UPROPERTY()
	uint8 bIsWieldingWeapon : 1;
	
	UPROPERTY()
	uint8 bIsFirst : 1;

	E_MovementSpeed MovementSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sword")
	TSubclassOf<AOVSword> SwordClass;
	
	//Montage
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;
	
	UFUNCTION()
	void OnDefaultAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UPROPERTY(EditAnywhere)
	E_AIState AIState;



public:
	UFUNCTION()
	virtual void SetState(E_AIState AIStateValue) override;

	UFUNCTION()
	virtual E_AIState GetState() override;
};
