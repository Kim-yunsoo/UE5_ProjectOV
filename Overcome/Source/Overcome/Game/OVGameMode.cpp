// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OVGameMode.h"
#include "OVGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"

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
	UGameplayStatics::LoadStreamLevel(this, "MainMap", true, true, LatentInfo) ;
}
