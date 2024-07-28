// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/OVItemBase.h"

#include "Component/OVInventoryComponent.h"

UOVItemBase::UOVItemBase() :bIsCopy(false),  bIsPickup(false)
{
}

void UOVItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UOVItemBase* UOVItemBase::CreateItemCopy() const  //원본을 복사한다. 그 사본을 리턴! 
{
	UOVItemBase* ItemCopy = NewObject<UOVItemBase>(StaticClass()); //정적 클래스를 만든다.
	ItemCopy->ID=this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->ItemNumericData = this->ItemNumericData;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->AssetData = this -> AssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
	
}
void UOVItemBase::SetQuantity(const int32 NewQuantity)
{
	if(NewQuantity!=Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity,0,ItemNumericData.bIsStackable? ItemNumericData.MaxStackSize:1);
		//bIsStackable가 true인경우 아닌경우

		if(OwningInventory)
		{
			if(Quantity<=0) //수량이 0과 같다면? 
			{
				OwningInventory->RemoveSingleInstanceOfItem(this); //수량이 0이면 인벤토리에서 아이템을 삭제한다. 
			} 
		}
		else
		{
			//UE_LOG(LogTemp,Error,TEXT("ItemBase OwningInventory was null! "));
		}
	}
}

void UOVItemBase::Use(AOVCharacterPlayer* Character)
{
}
