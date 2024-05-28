// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/OVInventoryTooltip.h"

#include "OVInventoryItemSlot.h"
#include "Components/TextBlock.h"
#include "Item/OVItemBase.h"

void UOVInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	const UOVItemBase* ItemBeginHovered = InventorySlotBeginHovered->GetItemReference();
	//아이템 슬롯의 아이템 정보를 가져온다. 

	switch (ItemBeginHovered->ItemType)
	{
	case EItemType::HPPotion:
		break;
	case EItemType::MPPotion:
		break;
	case EItemType::AttackPotion:
		break;
	case EItemType::Bullet:
		break;
	default: ;
	}

	ItemName->SetText(ItemBeginHovered->TextData.Name);
	UsageText->SetText(ItemBeginHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeginHovered->TextData.Description);

	if(ItemBeginHovered->ItemNumericData.bIsStackable)
	{
		MaxStackSize->SetText(FText::AsNumber(ItemBeginHovered->ItemNumericData.MaxStackSize));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
