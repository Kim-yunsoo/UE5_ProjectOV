// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun/OVGun.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/OVCharacterBase.h"
#include "Character/OVCharacterPlayer.h"

// Sets default values
AOVGun::AOVGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));	
	Mesh->SetupAttachment(Root);
	this->SetReplicates(true);
	this->SetActorEnableCollision(false);
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitRef(
TEXT("/Script/Niagara.NiagaraSystem'/Game/Vefects/Blood_VFX/VFX/Performance_Versions/Bullet_Hits/One_Shot/OS_NS_Bullet_Hit_Medium_Blue.OS_NS_Bullet_Hit_Medium_Blue'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}

}

void AOVGun::PullTrigger()
{
	UE_LOG(LogTemp, Log, TEXT("You've been shot!"));
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return;
	AController* OwnerController = OwnerPawn->GetController();
	if(OwnerController == nullptr) return;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MaxRange; 

	// DrawDebugPoint(GetWorld(), Location, 20, FColor::Red, true);
	// DrawDebugCamera(GetWorld(), Location, Rotation, 90, 2, FColor::Red, true);

	FHitResult Hit;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1);

	if (bSuccess)
	{
		FVector ShotDirection = -Rotation.Vector();
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		AOVCharacterBase* HitActor = Cast<AOVCharacterBase>(Hit.GetActor());
		if (HitActor != nullptr)
		{
			AOVCharacterPlayer* CharacterPlayer = Cast<AOVCharacterPlayer>(GetOwner());
			Damage = CharacterPlayer->GetAttack();
			UE_LOG(LogTemp, Warning, TEXT("Damage %f"), Damage);
			FPointDamageEvent DamageEvent{ Damage, Hit, ShotDirection, nullptr };
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EmitterHit, Hit.Location, FRotator::ZeroRotator);
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}


// Called when the game starts or when spawned
void AOVGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AOVGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

