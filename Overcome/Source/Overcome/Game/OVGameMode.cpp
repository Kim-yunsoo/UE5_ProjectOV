// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OVGameMode.h"
#include "OVGameState.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

AOVGameMode::AOVGameMode()
{
	GameStateClass = AOVGameState::StaticClass();
	
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Blueprint/Character/BP_OVCharacterPlayer.BP_OVCharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Overcome.OVPlayerController"));
	if (PlayerControllerClassRef.Class) {
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, "MainMap", true, false, LatentInfo) ;
	GoalCount = 0;
	Battery = 3;

	MainSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sound/Main.Main'"));
	AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), MainSound);
	BossSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sound/Boss.Boss'"));
}


void AOVGameMode::SetBatteryCount(int NewBattery)
{
	Battery = NewBattery;
	OnBatteryCount.Broadcast(Battery);
	if(Battery == GoalCount)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			UnloadOldLevel();
		}, 2.f, false);
	}
}

void AOVGameMode::ChangeLevel()
{
	FLatentActionInfo LoadLatentInfo;
	LoadLatentInfo.CallbackTarget = this;
	UGameplayStatics::LoadStreamLevel(this, "Boss", true, false, LoadLatentInfo);
}

void AOVGameMode::UnloadOldLevel()
{
	const FName LevelName = FName("ChangeLevel");
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.Linkage = 0;
	LatentActionInfo.ExecutionFunction = LevelName;
	UGameplayStatics::UnloadStreamLevel(GetWorld(), "MainMap", LatentActionInfo, false);
	AudioComponent->Stop();
	UGameplayStatics::PlaySound2D(GetWorld(), BossSound);
}
