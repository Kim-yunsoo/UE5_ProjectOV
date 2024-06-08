// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/OVOpenBossMap.h"

#include "Character/OVCharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOVOpenBossMap::AOVOpenBossMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Cylinder_F3406963.Cylinder_F3406963'"));
	if (MeshRef.Object)
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AOVOpenBossMap::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AOVOpenBossMap::OnTriggerEnter);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AOVOpenBossMap::OnTriggerExit);
}

void AOVOpenBossMap::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0); 
	if(MyCharacter)
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, "AI", true, true, LatentInfo) ;
	}

}

void AOVOpenBossMap::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0); 
	if(MyCharacter)
	{
	FLatentActionInfo LatentInfo;
	UGameplayStatics::UnloadStreamLevel(this, "MainMap", LatentInfo, true);
	}
}



