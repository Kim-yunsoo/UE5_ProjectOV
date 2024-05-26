// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Character/OVCharacterPlayer.h"
#include "Item/OVItemData.h"
#include "OVItemBase.generated.h"

/**
 * 
 */
class AOVCharacterPlayer;
UCLASS()
class OVERCOME_API UOVItemBase : public UObject
{
	GENERATED_BODY()

public:

	// UPROPERTY()
	// UInventoryComponent* OwningInventory;

	UPROPERTY(VisibleAnywhere,Category = "Item")
	int32 Quantity; //아이템 수량
	
	UPROPERTY(VisibleAnywhere,Category="Item")
	FName ID;

	UPROPERTY(VisibleAnywhere,Category="Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere,Category="Item")
	FItemStatistics ItemStatistics;

	UPROPERTY(VisibleAnywhere,Category="Item")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemNumericData ItemNumericData;

	UPROPERTY(VisibleAnywhere,Category="Item")
	FItemAssetData AssetData;

	UOVItemBase();

	UOVItemBase* CreateItemCopy() const;

	//FORCEINLINE float GetItemStackWeight() const{return Quantity* ItemNumericData.Weight};  무게 코드
	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == ItemNumericData.MaxStackSize;}; // 수량이 최대 스택크기와 같은지?

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);
	
	UFUNCTION(Category = "Item") //캐리터를 가져온다. 
	virtual void Use(AOVCharacterPlayer* Character);

protected:
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
};
