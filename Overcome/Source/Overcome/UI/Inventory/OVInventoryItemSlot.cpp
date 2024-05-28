// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/OVInventoryItemSlot.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/OVItemBase.h"
#include "UI/Inventory/OVInventoryTooltip.h"

void UOVInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(ToolTipClass)
	{
		UOVInventoryTooltip* ToolTip = CreateWidget<UOVInventoryTooltip>(this, ToolTipClass);
		ToolTip->InventorySlotBeginHovered =this;
		SetToolTip(ToolTip); //앞서 생성한 위젯을 툴팁으로 사용하려 설정함 
	}
}

void UOVInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if(ItemReference)
	{
		switch (ItemReference->ItemType)
		{
		case EItemType::HPPotion:
			ItemBorder->SetBrushColor(FLinearColor::Green);
			break;
		case EItemType::MPPotion:
			ItemBorder->SetBrushColor(FLinearColor::Blue);
			break;
		case EItemType::AttackPotion:
			ItemBorder->SetBrushColor(FLinearColor::Red);
			break;
		case EItemType::Bullet:
			ItemBorder->SetBrushColor(FLinearColor::Gray);
			break;
		}
		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

		if(ItemReference->ItemNumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity)); // 숫자 텍스트 AsNumber
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UOVInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UOVInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UOVInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UOVInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
