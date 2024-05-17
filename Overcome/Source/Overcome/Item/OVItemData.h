// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OVItemData.generated.h"

UENUM()
enum class EItemType : UINT8
{
	HpPotion = 0,
	MpPotion,
	AttackPotion,
	Damage
};

/**
 * 
 */


UCLASS()
class OVERCOME_API UOVItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;
};
