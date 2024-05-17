// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OVSkillBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSkillCharacter, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOME_API UOVSkillBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOVSkillBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	float CoolTime;

	UPROPERTY()
	float Mp;
	
	UPROPERTY()
	TObjectPtr<UTexture> SkillTexture;
	
	UPROPERTY()
	TObjectPtr<class AOVCharacterPlayer> Owner;
	
public:
	float ActivetedTimeStamp;
	
	UPROPERTY()
	TObjectPtr<USoundWave> SkillSound;
	
	virtual void SkillAction();
};
