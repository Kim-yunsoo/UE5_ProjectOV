// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "OVItemData.generated.h"

UENUM()
enum class EItemType : uint8 //무기 종류
{
	HPPotion UMETA(DisplayName = "HPPotion"),
	MPPotion UMETA(DisplayName = "MPPotion"),
	AttackPotion UMETA(DisplayName = "AttackPotion"),
	Bullet UMETA(DisplayName = "Bullet"),
	Battery UMETA(DisplayName = "Battery")
};

USTRUCT()
struct FItemStatistics // 아이템 효과 
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnyWhere)
	float DamageValue;
	
};

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere)
	FText Name;
	UPROPERTY(EditAnyWhere)
	FText Description;
	UPROPERTY(EditAnyWhere)
	FText InteractionText;
	UPROPERTY(EditAnyWhere)
	FText UsageText;
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;
	
	UPROPERTY(EditAnywhere)
	bool bIsStackable;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnyWhere)
	UTexture2D* Icon;
	UPROPERTY(EditAnyWhere)
	UStaticMesh* Mesh;
};

USTRUCT()
struct FItemData: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnyWhere,Category="Item Data")
	FName ID;

	UPROPERTY(EditAnyWhere,Category="Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnyWhere,Category="Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnyWhere,Category="Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData ItemNumericData;

	UPROPERTY(EditAnyWhere,Category="Item Data")
	FItemAssetData AssetData;
};
