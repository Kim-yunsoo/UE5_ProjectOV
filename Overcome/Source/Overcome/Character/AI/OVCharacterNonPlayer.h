// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OVCharacterBase.h"
#include "Interface/OVCharacterAIInterface.h"
#include "Interface/OVDamagableInterface.h"
#include "OVCharacterNonPlayer.generated.h"

UENUM(BlueprintType)
enum class E_Item : uint8
{
	//None UMETA(DisplayName = "None"),
	HPPotion UMETA(DisplayName = "HPPotion"),
	MPPotion UMETA(DisplayName = "MPPotion"),
	AttackPotion UMETA(DisplayName = "AttackPotion")
};


/**
 * 
 */
UCLASS()
class OVERCOME_API AOVCharacterNonPlayer : public AOVCharacterBase, public IOVCharacterAIInterface, public IOVDamagableInterface
{
	GENERATED_BODY()
public:
	AOVCharacterNonPlayer();
	
	// AI Section
protected:
	
	
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyActionEnd() override;

	FString GetRandomItemName();

	UPROPERTY()
	FString RandomItemName;
	
	//Dead Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	float DeadEventDelayTime = 3.0f;
	void SetDead() override;
	void PlayDeadAnimation();

	virtual void BeginPlay() override;

	//Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> StaggerMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UOVDamageComponent> DamageComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UOVAttackComponent> AttackComponent;

	virtual void PostInitializeComponents() override;
	
	//Damage
	virtual float GetCurrentHealth() override;
	virtual float GetMaxHealth() override;
	virtual float Heal(float Amount) override; 
	virtual bool TakeDamage(FDamageInfo DamageInfo) override;
	virtual bool IsDead() override;
	virtual bool IsAttacking() override;

	UPROPERTY()
	uint8 bIsAttacking : 1;

	void DamageResponse(E_DamageResponses DamageResponses);
};
