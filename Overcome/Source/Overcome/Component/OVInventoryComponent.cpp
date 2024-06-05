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
FItemAddResult UOVInventoryComponent::HandleAddItem(UOVItemBase* InputItem)
{
	if(GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// non-stackable items
		if(!InputItem->ItemNumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem);
		}

		//handle stackable
		const int32 StackableAmountAdded = HandleStackableItems(InputItem,InitialRequestedAddAmount);
		//쌓을 수 있는 정도를 알려줌 

		if(StackableAmountAdded == InitialRequestedAddAmount) //같으면 모두 넣을 수 있다.
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount,FText::Format(
				FText::FromString("Successfully added {0} {1} to the inventory."),
				InitialRequestedAddAmount,
				InputItem->TextData.Name
				));
		}

		if(StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0) //부분적으로만 넣을 수 있다
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded,FText::Format(
				FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"),
				InputItem->TextData.Name,
				StackableAmountAdded));
		}
		if(StackableAmountAdded<=0) //넣지 못한다, 
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. No Remaining inventory slots, or invalid item"),
				InputItem->TextData.Name));
		}
	}
	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed"));
	
}
FItemAddResult UOVInventoryComponent::HandleNonStackableItems(UOVItemBase* InputItem)
{
	//인벤토리 슬롯이 꽉찬 경우
	 if(InventoryContents.Num()+1 > InventorySlotsCapacity)
	 {
	 	return FItemAddResult::AddedNone(FText::Format(
	 		FText::FromString("Could not add {0} to the inventory. All inventory slots are full."),InputItem->TextData.Name));
	 }

	AddNewItem(InputItem,1);
	return FItemAddResult::AddedAll(1,FText::Format(
		FText::FromString("Successfully added a single {0} to the inventory. "),InputItem->TextData.Name));
}

int32 UOVInventoryComponent::HandleStackableItems(UOVItemBase* ItemIn, int32 RequestedAddAmount)
{
	if(RequestedAddAmount<=0)
	{
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount;

	UOVItemBase* ExistingItem = FindNextPartialStack(ItemIn);

	while (ExistingItem) //부분적으로 넣을 스택이 있다면?
	{
		//const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItem, AmountToDistribute);
		//int32 RemainAmount = ExistingItem->ItemNumericData.MaxStackSize
		int32 CalCulateNumber = ExistingItem->ItemNumericData.MaxStackSize - ExistingItem->Quantity; // 수용될 수 있는 양
		if(CalCulateNumber >= RequestedAddAmount) //수용될 수 있는 양이 요청 값보단 많다면? 
		{
			ExistingItem->SetQuantity(ExistingItem->Quantity + RequestedAddAmount); //요청한거 그대로 올려두기
			ItemIn->SetQuantity(0); //바닥에 남은 것 -> 다 줍기! 

			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount; // 전부 추가하도록 요청 금액 리턴 
		}
		else
		{
			int32 RemainItemAmount = RequestedAddAmount - CalCulateNumber; //바닥에 남을 값
			ExistingItem->SetQuantity(ExistingItem->Quantity + CalCulateNumber); //요청한거 그대로 올려두기
			ItemIn->SetQuantity(RemainItemAmount);

			OnInventoryUpdated.Broadcast();
			return CalCulateNumber; // 전부 추가하도록 요청 금액 리턴 
			
		}
	}

	if(InventoryContents.Num()+1 <= InventorySlotsCapacity) // 다 넣을 수 있을 경우와 인벤토리 용량이 남은 경우 
	{
		//const int32 AddAmount = RequestedAddAmount;
		ItemIn->SetQuantity(0);  //바닥에 남은 것 -> 다 줍기!
		AddNewItem(ItemIn->CreateItemCopy(),RequestedAddAmount);
		OnInventoryUpdated.Broadcast();
		return RequestedAddAmount;
	}
	return 0;
}

int32 UOVInventoryComponent::CalculateNumberForFullStack(UOVItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->ItemNumericData.MaxStackSize - StackableItem->Quantity;
	// 스택, 
	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UOVInventoryComponent::AddNewItem(UOVItemBase* Item, const int32 AmountToAdd)
{
	UOVItemBase* NewItem;

	if(Item->bIsCopy || Item->bIsPickup)
	{
		//이미 아이템이 복사된 상태고, 그 아이템을 월드에서 주울 경우
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// 스플릿 할 때나 다른 인벤토리로 드래그 할 때 사용된다. 
		NewItem = Item->CreateItemCopy();
	}
	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);
	
	InventoryContents.Add(NewItem);
	OnInventoryUpdated.Broadcast();
}


