// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVHUDWidget.h"

#include "OVBossHpWidget.h"
#include "UI/OVMainMenu.h"
#include "UI/Interaction/OVInteractionWidget.h"
#include "OVStatWidget.h"
#include "OVTargetWidget.h"
#include "Components/VerticalBox.h"
#include "Game/OVGameState.h"
#include "Interface/OVCharacterHUDInterface.h"
#include "Kismet/GameplayStatics.h"

UOVHUDWidget::UOVHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{       
}

void UOVHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TargetWidget = Cast<UOVTargetWidget>(GetWidgetFromName(TEXT("WBP_TargetWidget")));
	StatWidget = Cast<UOVStatWidget>(GetWidgetFromName(TEXT("WBP_Stat")));
	BossHpWidget = Cast<UOVBossHpWidget>(GetWidgetFromName(TEXT("WBP_BossHpBar")));
	TeleportSkillWidget = Cast<UOVStatWidget>(GetWidgetFromName("WBP_Stat"));
	ShieldSkillWidget = Cast<UOVStatWidget>(GetWidgetFromName("WBP_Stat"));
	//OnBossAttackState.AddDynamic(this, &UOVHUDWidget::UpdateBossUI);
	// InteractionWidget = Cast<UOVInteractionWidget>(GetWidgetFromName(TEXT("WBP_InteractionWidget")));
	// if(InteractionWidget)
	// {
	// 	InteractionWidget->AddToViewport(-1);
	// 	InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	// }

	if(InteractionWidgetClass)
	{
		InteractionWidget= CreateWidget<UOVInteractionWidget>(GetWorld(),InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(MainMenuClass)
	{
		MainMenuWidget= CreateWidget<UOVMainMenu>(GetWorld(),MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	
	IOVCharacterHUDInterface* CharacterWidget = Cast<IOVCharacterHUDInterface>(GetOwningPlayerPawn());
	if(CharacterWidget)
	{
		CharacterWidget->SetupHUDWidget(this);
	}
	if (AOVGameState* GameState = Cast<AOVGameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		GameState->OnBossAttackState.AddDynamic(this, &UOVHUDWidget::UpdateBossUI);
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

void UOVHUDWidget::ToggleMenu()
{
	if(bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(true);
	}
}

void UOVHUDWidget::ShowInteractionWidget() const
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UOVHUDWidget::HideInteractionWidget() const
{
	if(InteractionWidget)
	{
		//UE_LOG(LogTemp,Log,TEXT("%s"),*InteractionWidget->GetName() )
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void UOVHUDWidget::UpdateInteractionWidget(const FInteractableData* InteractionData) const
{
	if(InteractionWidget)
	{
		if(InteractionWidget->GetVisibility()==ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

		InteractionWidget->UpdateWidget(InteractionData);
	}
}

void UOVHUDWidget::UpdateTarget(bool bIsShowUI) const
{
	TargetWidget->UpdateTargetUI(bIsShowUI);
}

void UOVHUDWidget::UpdateBossUI(bool bIsShowUI)
{
	BossHpWidget->UpdateBossUI(bIsShowUI);
}

void UOVHUDWidget::UpdateTeleportTime(float NewCurrentTime)
{
	TeleportSkillWidget->UpdateTeleportBar(NewCurrentTime);
}

void UOVHUDWidget::UpdateShieldTime(float NewCurrentTime)
{
	ShieldSkillWidget->UpdateShieldBar(NewCurrentTime);
}