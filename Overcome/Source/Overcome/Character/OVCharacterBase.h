// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/OVAnimationAttackInterface.h"
#include "Interface/OVCharateWidgetInterface.h"
#include "OVCharacterBase.generated.h"


class UNiagaraSystem;

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class OVERCOME_API AOVCharacterBase : public ACharacter, public IOVCharateWidgetInterface, public IOVAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOVCharacterBase();

	virtual void PostInitializeComponents() override;
	
protected:
	virtual void SetCharacterControlData(const class UOVCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UOVCharacterControlData*> CharacterControlManager;

//Gun Section
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// UPROPERTY(EditDefaultsOnly, Category = Gun)
	// float MaxHealth = 100.f;

	// UPROPERTY(VisibleAnywhere)
	// float Health;

	UFUNCTION()
	virtual void SetDead();
	
	//Stat Section
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UOVCharacterStatComponent> Stat;
	
	//UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UOVWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(UOVUserWidget* InUserWidget) override;


	//Attack Hit Section
protected:
	virtual void AttackHitCheck() override;

	virtual void NotifyActionEnd();
};
