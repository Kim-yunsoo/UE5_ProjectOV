// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/OVInventoryItemSlot.h"
#include "OVItemDragDropOperation.h"
#include "Character/OVCharacterPlayer.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/OVItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Inventory/OVInventoryTooltip.h"
#include "UI/Inventory/OVDragItemVisual.h"

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
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(),EKeys::LeftMouseButton);
	}
	else if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if(ItemReference->ItemType == EItemType::Battery)
			return Reply.Unhandled();
		AOVCharacterPlayer* PlayerCharacter = Cast<AOVCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (PlayerCharacter)
		{
			UOVItemBase* ItemToUse = ItemReference;
			int32 QuantityToUse = 1;
			PlayerCharacter->ItemUse(ItemToUse, QuantityToUse);
		}
		return Reply.Handled();
	}
	return Reply.Unhandled();
}

void UOVInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UOVInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, //끌기를 감지한다.
	//마우스 클릭 자체를 감지하진 않는다. 
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if(DragItemVisualClass)
	{
		const TObjectPtr<UOVDragItemVisual> DragVisual = CreateWidget<UOVDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());
		DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));

		UOVItemDragDropOperation* DragItemOperation = NewObject<UOVItemDragDropOperation>();
		DragItemOperation->SourceItem =ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;

		DragItemOperation->DefaultDragVisual = DragVisual; // TObjectPtr 의 경우 가능
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation; //Main에서 Drop할때 InOperation의 인자 값와 동일한 종류가 된다. 그래서 캐스트에 성공할 수 있다.
		
	}
}

bool UOVInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

