// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVHUDWidget.h"

#include "OVMainMenu.h"
#include "Interaction/OVInteractionWidget.h"

#include "OVStatWidget.h"
#include "OVTargetWidget.h"
#include "Interface/OVCharacterHUDInterface.h"

UOVHUDWidget::UOVHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{       
}

void UOVHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TargetWidget = Cast<UOVTargetWidget>(GetWidgetFromName(TEXT("WBP_TargetWidget")));
	StatWidget = Cast<UOVStatWidget>(GetWidgetFromName(TEXT("WBP_Stat")));
	IOVCharacterHUDInterface* CharacterWidget = Cast<IOVCharacterHUDInterface>(GetOwningPlayerPawn());
	if(CharacterWidget)
	{
		CharacterWidget->SetupHUDWidget(this);
	}

	if(MainMenuClass)
	{
		MainMenuWidget= CreateWidget<UOVMainMenu>(GetWorld(),MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(InteractionWidgetClass)
	{
		InteractionWidget= CreateWidget<UOVInteractionWidget>(GetWorld(),InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UOVHUDWidget::DisplayMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible=true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UOVHUDWidget::HideMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible=false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UOVHUDWidget::ShowInteractionWidget()
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UOVHUDWidget::HideInteractionWidget()
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UOVHUDWidget::UpdateInteractionWidget(const FInteractionData* InteractionData)
{
	if(InteractionWidget)
	{
		if(InteractionWidget->GetVisibility()==ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		// InteractionWidget->UpdateWidget(InteractableData);
	}
}

void UOVHUDWidget::UpdateTarget(bool bIsShowUI)
{
	TargetWidget->UpdateTargetUI(bIsShowUI);
}
