// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OVCharacterBase.h"
#include "Interface/OVCharacterAIInterface.h"
#include "OVCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API AOVCharacterNonPlayer : public AOVCharacterBase, public IOVCharacterAIInterface
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
	
	//Dead Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	float DeadEventDelayTime = 3.0f;
	void SetDead() override;
	void PlayDeadAnimation();

	virtual void BeginPlay() override;

	//Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

};
