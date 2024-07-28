// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword/OVSword.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AOVSword::AOVSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	SetRootComponent(Root);
	Mesh->SetupAttachment(Root);
	//SetRootComponent(Mesh);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DarkMetalPack/DarkMetalOniCyclone/ONISaber/SM_ONISaber.SM_ONISaber'"));
	if (MeshRef.Object)
	{
		//UE_LOG(LogTemp, Warning, TEXT("MESH"));
		Mesh->SetSkeletalMesh(MeshRef.Object);
	}
	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));


}

// Called when the game starts or when spawned
void AOVSword::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AOVSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

