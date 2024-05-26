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
		}
		))
	{
		return *Result;
	}
}

void UOVInventoryComponent::RemoveSingleInstanceOfItem(UOVItemBase* ItemIn)
{
}

int32 UOVInventoryComponent::RemoveAmountOfItem(UOVItemBase* ItemIn, int32 DesiredAmountToRemove)
{
}

void UOVInventoryComponent::SplitExistingStack(UOVItemBase* ItemIn, const int32 AmountToSplit)
{
}

FItemAddResult UOVInventoryComponent::HandleNonStackableItems(UOVItemBase*, int32 RequestedAddAmount)
{
}

int32 UOVInventoryComponent::HandleStackableItems(UOVItemBase*, int32 RequestedAddAmount)
{
}

int32 UOVInventoryComponent::CalculateNumberForFullStack(UOVItemBase* ExistingItem, int32 InitialRequestedAddAmount)
{
}

void UOVInventoryComponent::AddNewItem(UOVItemBase* Item, const int32 AmountToAdd)
{
}


