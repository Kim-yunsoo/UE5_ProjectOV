// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVHUDWidget.h"

#include "OVBatteryWidget.h"
#include "OVBossHpWidget.h"
#include "OVDeadWidget.h"
#include "OVResumeWidget.h"
#include "UI/OVMainMenu.h"
#include "UI/Interaction/OVInteractionWidget.h"
#include "OVStatWidget.h"
#include "OVTargetWidget.h"
#include "Components/VerticalBox.h"
#include "Game/OVGameMode.h"
#include "Game/OVGameState.h"
#include "Interface/OVCharacterHUDInterface.h"
#include "Kismet/GameplayStatics.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

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
	GunSkillWidget = Cast<UOVStatWidget>(GetWidgetFromName("WBP_Stat"));
	BatteryWidget = Cast<UOVBatteryWidget>(GetWidgetFromName("WBP_Battery"));
	DeadWidget = Cast<UOVDeadWidget>(GetWidgetFromName("WBP_Dead"));
	ResumeWidget = Cast<UOVResumeWidget>(GetWidgetFromName("WBP_Resume"));
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
	Battery = 5;
	bIsMenuVisible  = false;
	if(DeadWidget)
		DeadWidget->SetVisibility(ESlateVisibility::Hidden);

	if(ResumeWidget)
	{
		ResumeWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	bIsfirst = true;
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
	if(bIsMenuVisible) //화면 보이는 중
	{
		HideMenu(); //화면 사라짐
		const FInputModeGameOnly InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(false);
	}
	else //화면 안보임
	{
		DisplayMenu(); //화면 보이게
		const FInputModeGameAndUI InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(true);
	}
}

void UOVHUDWidget::ResumeMenu()
{
	// if(bIsResumeMenuVisible)
	// {
	// 	ResumeWidget->SetVisibility(ESlateVisibility::Hidden);
	// 	const FInputModeGameOnly InputMode;
	// 	GetOwningPlayer()->SetInputMode(InputMode);
	// 	GetOwningPlayer()->SetShowMouseCursor(false);
	// }
	// else
	{
		ResumeWidget->SetVisibility(ESlateVisibility::Visible);
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

void UOVHUDWidget::UpdateGunTime(float NewCurrentTime)
{
	GunSkillWidget->UpdateGunBar(NewCurrentTime);
}

void UOVHUDWidget::UpdateBatteryCount(int NewCount)
{
	BatteryWidget->UpdateBatteryCount(NewCount);
}

void UOVHUDWidget::UpdateDead()
{
	DeadWidget->SetVisibility(ESlateVisibility::Visible);
	AOVGameState* GameState = Cast<AOVGameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->SetCharacterDead(true);
	const FInputModeGameAndUI InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);
}

void UOVHUDWidget::Ending()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		if(bIsfirst)
		{
			bIsfirst = false;
			EndingWidget = CreateWidget<UUserWidget>(this, EndingWidgetClass);
			if (EndingWidget)
			{
				EndingWidget->AddToViewport(10);
				const FInputModeUIOnly InputMode;
				GetOwningPlayer()->SetInputMode(InputMode);
				GetOwningPlayer()->SetShowMouseCursor(true);
				EndingWidget->SetVisibility(ESlateVisibility::Visible);
			}

			TargetWidget->SetVisibility(ESlateVisibility::Hidden);
			StatWidget->SetVisibility(ESlateVisibility::Hidden);
			BossHpWidget->SetVisibility(ESlateVisibility::Hidden);
			TeleportSkillWidget->SetVisibility(ESlateVisibility::Hidden);
			BatteryWidget->SetVisibility(ESlateVisibility::Hidden);
		
		}
	}, 1.0f, false);
}


