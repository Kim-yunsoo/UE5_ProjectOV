// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/OVNonPlayerSpawner.h"
#include "Components/BoxComponent.h"

AOVNonPlayerSpawner::AOVNonPlayerSpawner()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetCollisionProfileName(TEXT("NoCollision"));

	RootComponent = Trigger;
	NonPlayerCount = 10;
	bIsTrue = false;
}

void AOVNonPlayerSpawner::BeginPlay()
{
	Super::BeginPlay();

	if(!bIsTrue)
		return;
	float MaxX = GetActorLocation().X + Trigger->GetScaledBoxExtent().X;
	float MaxY = GetActorLocation().Y + Trigger->GetScaledBoxExtent().Y;

	float MinX = GetActorLocation().X - Trigger->GetScaledBoxExtent().X;
	float MinY = GetActorLocation().Y - Trigger->GetScaledBoxExtent().Y;

	for (int32 Idx = 0; Idx < NonPlayerCount; Idx++)
	{
		float X = FMath::RandRange(MinX, MaxX);
		float Y = FMath::RandRange(MinY, MaxY);
		FVector Location(X, Y, GetActorLocation().Z);
		FActorSpawnParameters Params;
		Params.Owner = this;
		//UE_LOG(LogTemp, Warning, TEXT("Spawn AI"));
		AOVCharacterNonPlayer* Item = GetWorld()->SpawnActor<AOVCharacterNonPlayer>(
			NonPlayer, Location, FRotator::ZeroRotator, Params);
	}
}

