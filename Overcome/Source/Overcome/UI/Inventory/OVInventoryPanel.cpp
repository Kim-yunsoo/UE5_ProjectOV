// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/OVInventoryPanel.h"

#include "OVInventoryItemSlot.h"
#include "OVItemDragDropOperation.h"
#include "Character/OVCharacterPlayer.h"
#include "Component/OVInventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Item/OVItemBase.h"

void UOVInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	PlayerCharacter = Cast<AOVCharacterPlayer>(GetOwningPlayerPawn());

	if(PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if(InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UOVInventoryPanel::RefreshInventory);
			//인벤토리 컴포넌트 델리게이트와 연결
			SetInfoText();
		}
	}
}
void UOVInventoryPanel::SetInfoText() const
{
	CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
		InventoryReference->GetInventoryContents().Num(),
		InventoryReference->GetSlotsCapacity()));
}

void UOVInventoryPanel::RefreshInventory()
{
	if(InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren(); // 추가하기 전에 자식 UI를 정리

		for(UOVItemBase* const& InventoryItem : InventoryReference ->GetInventoryContents())
		{
			UOVInventoryItemSlot* ItemSlot = CreateWidget<UOVInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);
			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}
		SetInfoText();
	}
}


bool UOVInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	//return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	const UOVItemDragDropOperation* ItemDragDrop = Cast<UOVItemDragDropOperation>(InOperation);

	if(ItemDragDrop->SourceItem && InventoryReference)
	{
		UE_LOG(LogTemp,Warning,TEXT("Detected an item drop on InventoryPanel"));
		return true;
	}
	return false;
}
