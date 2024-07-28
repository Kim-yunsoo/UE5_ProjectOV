// Fill out your copyright notice in the Description page of Project Settings.
#include "Gun/OVGun.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/OVCharacterBase.h"
#include "Character/OVCharacterPlayer.h"
#include "Character/AI/OVCharacterNonPlayer.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

// Sets default values
AOVGun::AOVGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	SetRootComponent(Root);
	Mesh->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Gun/rifle.rifle'"));
	if (MeshRef.Object)
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}
	this->SetReplicates(true);
	this->SetActorEnableCollision(false);
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitRef(
TEXT("/Script/Niagara.NiagaraSystem'/Game/Vefects/Blood_VFX/VFX/Performance_Versions/Bullet_Hits/One_Shot/OS_NS_Bullet_Hit_Medium_Blue.OS_NS_Bullet_Hit_Medium_Blue'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> MuzzleEffectRef(
TEXT("/Script/Niagara.NiagaraSystem'/Game/Vefects/Shots_VFX/VFX/MuzzleFlash/Looped/FX_MuzzleFlash_Rifle_Custom.FX_MuzzleFlash_Rifle_Custom'"));

	if (MuzzleEffectRef.Succeeded())
	{
		MuzzleEffect = MuzzleEffectRef.Object;
	}
	BulletCount = 20;
}

void AOVGun::PullTrigger()
{
	BulletCount -= 1;
	//UE_LOG(LogTemp, Log, TEXT("You've been shot! %d"), BulletCount);
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
	UStaticMeshComponent* WeaponMesh = Mesh; // 총의 메쉬를 반환하는 함수
	if (WeaponMesh)
	{
		FName SocketName = TEXT("Socket"); // 소켓 이름
		FVector MuzzleLocation = WeaponMesh->GetSocketLocation(SocketName);
		FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(SocketName);

		// 나이아가라 시스템을 소켓 위치에서 스폰
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleEffect, MuzzleLocation, MuzzleRotation);
		UE_LOG(LogTemp, Warning, TEXT("HAPPY"));
	}
	UE_LOG(LogTemp, Warning, TEXT("HAPPY"));

	FHitResult Hit;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1);

	if (bSuccess) //이득우 교수님 AI 플레이어
	{
		
		FVector ShotDirection = -Rotation.Vector();
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		AOVCharacterNonPlayer* HitActor = Cast<AOVCharacterNonPlayer>(Hit.GetActor());
		if (HitActor != nullptr)
		{
			AOVCharacterPlayer* CharacterPlayer = Cast<AOVCharacterPlayer>(GetOwner());
			Damage = CharacterPlayer->GetAttack();
			//UE_LOG(LogTemp, Warning, TEXT("Damage %f"), Damage);
			FPointDamageEvent DamageEvent{ Damage, Hit, ShotDirection, nullptr };
			//HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			MulticastEffect(Hit.Location);
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

void AOVGun::MulticastEffect_Implementation(FVector Location)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EmitterHit, Location, FRotator::ZeroRotator);
}

