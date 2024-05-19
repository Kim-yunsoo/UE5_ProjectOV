// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/OVInterfaceTestActor.h"

// Sets default values
AOVInterfaceTestActor::AOVInterfaceTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(Mesh);

}

// Called when the game starts or when spawned
void AOVInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
}

// Called every frame
void AOVInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	
}

void AOVInterfaceTestActor::BeginFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AOVInterfaceTestActor::EndFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void AOVInterfaceTestActor::BeginInteract()
{
	UE_LOG(LogTemp,Warning,TEXT("Calling BeginInteract override on interface test Actor"));
}

void AOVInterfaceTestActor::EndInteract()
{
	UE_LOG(LogTemp,Warning,TEXT("Calling EndInteract override on interface test Actor"));
}

void AOVInterfaceTestActor::Interact(AOVCharacterPlayer* PlayerCharacter)
{
	UE_LOG(LogTemp,Warning,TEXT("Calling Interact override on interface test Actor"));
}

