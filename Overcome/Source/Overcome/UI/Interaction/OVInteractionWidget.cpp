// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Interaction/OVInteractionWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/OVInteractionInterface.h"

void UOVInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this,"UpdateInteractionProgress");
}

void UOVInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	KeyPressText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.0f;
}

void UOVInteractionWidget::UpdateWidget(const FInteractableData* InteractableData) const
{
	switch (InteractableData->InteractableType)
	{
	case EInteractableType::Pickup:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		if(InteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			QuantityText->SetText(FText::Format(NSLOCTEXT("InteractionWidget","QuantityText","x{0}"),
				InteractableData->Quantity));
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		break;
	case EInteractableType::NonPlayerCharacter:
		break;
	case EInteractableType::Device:
		break;
	case EInteractableType::Toggle:
		break;
	case EInteractableType::Container:
		break;
	default: ;
	}

	ActionText->SetText(InteractableData->Action);
	NameText->SetText(InteractableData->Name);
}

float UOVInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}

