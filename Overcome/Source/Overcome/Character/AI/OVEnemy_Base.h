// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OVCharacterBase.h"
#include "Interface/OVEnemyAIInterface.h"
#include "OVEnemy_Base.generated.h"

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

	//Montage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;

	UFUNCTION()
	void OnDefaultAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
