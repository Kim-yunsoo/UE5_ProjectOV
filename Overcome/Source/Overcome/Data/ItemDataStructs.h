// #pragma once
//
// #include "CoreMinimal.h"
// #include "Engine/DataTable.h"
// #include "ItemDataStructs.generated.h"
//
//
// UENUM()
// enum class EItemQuality :uint8 //아이템이 어떤 상태인지 열거형 
// {
// 	Shoddy UMETA(DisplayName = "Shoddy"),
// 	Common UMETA(DisplayName = "Common"),
// 	Quality UMETA(DisplayName = "Quality"),
// 	Masterwork UMETA(DisplayName = "Masterwork"),
// 	Grandmaster UMETA(DisplayName = "Grandmaster"),
// };
//
// UENUM()
// enum class EItemType : uint8 //무기 종류
// {
// 	Armor UMETA(DisplayName = "Armor"),
// 	Weapon UMETA(DisplayName = "Armor"),
// 	Shield UMETA(DisplayName = "Armor"),
// 	Spell UMETA(DisplayName = "Armor"),
// };
//
// USTRUCT()
// struct FItemStatistics // 아이템 효과
// {
// 	GENERATED_USTRUCT_BODY()
// 	
// 	UPROPERTY(EditAnyWhere)
// 	float DamageValue;
// 	
// };
//
// USTRUCT()
// struct FItemTextData
// {
// 	GENERATED_USTRUCT_BODY()
//
// 	UPROPERTY(EditAnyWhere)
// 	FText Name;
// 	UPROPERTY(EditAnyWhere)
// 	FText Description;
// 	UPROPERTY(EditAnyWhere)
// 	FText InteractionText;
// 	UPROPERTY(EditAnyWhere)
// 	FText UsageText;
// };
//
// USTRUCT()
// struct FItemAssetData
// {
// 	GENERATED_USTRUCT_BODY()
// 	
// 	UPROPERTY(EditAnyWhere)
// 	UTexture2D* Icon;
// 	UPROPERTY(EditAnyWhere)
// 	UStaticMesh* Mesh;
// };
//
// USTRUCT()
// struct FItemData: public FTableRowBase
// {
// 	GENERATED_USTRUCT_BODY()
//
// 	
//
// 	
// 	
// };
