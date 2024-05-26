// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/OVInventoryComponent.h"

#include "Item/OVItemBase.h"

// Sets default values for this component's properties
UOVInventoryComponent::UOVInventoryComponent()
{
}

// Called when the game starts
void UOVInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

FItemAddResult UOVInventoryComponent::HandleAddItem(UOVItemBase* InputItem)
{
}

UOVItemBase* UOVInventoryComponent::FindMatchingItem(UOVItemBase* ItemIn) const
{
	if(ItemIn)
	{
		if(InventoryContents.Contains(ItemIn)) //포함이 되어있는지 ? 
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UOVItemBase* UOVInventoryComponent::FindNextItemByID(UOVItemBase* ItemIn) const
{
	if(ItemIn)
	{
		if(const TArray<TObjectPtr<UOVItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{// 배열에서 항목을 찾는다. 
			return *Result;
		}
	}

	return nullptr;
	
}

UOVItemBase* UOVInventoryComponent::FindNextPartialStack(UOVItemBase* ItemIn) const
{
	if(const TArray<TObjectPtr<UOVItemBase>>::ElementType* Result =
		InventoryContents.FindByPredicate([&ItemIn](const UOVItemBase* InventoryItem)
		{
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
			//반환 조건을 정의한다. 
		}
		))
	{
		return *Result;			//ItemIn->ID와 일치하고, 완전히 채워지지 않은 아이템 스택을 배열에서 찾아서 반환한다.
	}

	return nullptr; // 찾지못하면 nullptr 반환. 
}

void UOVInventoryComponent::RemoveSingleInstanceOfItem(UOVItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UOVInventoryComponent::RemoveAmountOfItem(UOVItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);
	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
	
}

void UOVInventoryComponent::SplitExistingStack(UOVItemBase* ItemIn, const int32 AmountToSplit)
{
	if(!(InventoryContents.Num() + 1 > InventorySlotsCapacity)) //인벤토리 용량이 넘지 않을 때 
	{
		RemoveAmountOfItem(ItemIn,AmountToSplit); //일부를 제거하고
		AddNewItem(ItemIn, AmountToSplit); //분할 
	}

}

FItemAddResult UOVInventoryComponent::HandleNonStackableItems(UOVItemBase*, int32 RequestedAddAmount)
{
}

int32 UOVInventoryComponent::HandleStackableItems(UOVItemBase*, int32 RequestedAddAmount)
{
}

int32 UOVInventoryComponent::CalculateNumberForFullStack(UOVItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->ItemNumericData.MaxStackSize - StackableItem->Quantity;
	//스택 남은 것
	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UOVInventoryComponent::AddNewItem(UOVItemBase* Item, const int32 AmountToAdd)
{
}


