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

UOVItemBase* UOVItemBase::CreateItemCopy() const  //������ �����Ѵ�. �� �纻�� ����! 
{
	UOVItemBase* ItemCopy = NewObject<UOVItemBase>(StaticClass()); //���� Ŭ������ �����.
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
		//bIsStackable�� true�ΰ�� �ƴѰ��

		if(OwningInventory)
		{
			if(Quantity<=0) //������ 0�� ���ٸ�? 
			{
				OwningInventory->RemoveSingleInstanceOfItem(this); //������ 0�̸� �κ��丮���� �������� �����Ѵ�. 
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
