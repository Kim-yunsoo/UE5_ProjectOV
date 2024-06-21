// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVCharacterNonPlayer.h"

#include "BrainComponent.h"
#include "OVAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Object/OVPickup.h"
#include "Component/OVAttackComponent.h"
#include "Component/OVCharacterStatComponent.h"
#include "Component/OVDamageComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/OVGameMode.h"
#include "Game/OVGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/OVCollision.h"
#include "UI/OVWidgetComponent.h"

AOVCharacterNonPlayer::AOVCharacterNonPlayer()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.f), FRotator(0.0f, -90.0f, 0.0f));
	// //GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CPROFILE_OVCAPSULE"));
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharaterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DarkMetalPack/ZombotMike/Mesh/SM_Zombot_Mikey.SM_Zombot_Mikey'"));
	if (CharaterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharaterMeshRef.Object); 
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/DarkMetalPack/ZombotMike/Animations/Anim_Zombot_Mikey_Death_Montage.Anim_Zombot_Mikey_Death_Montage'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/DarkMetalPack/ZombotMike/Animations/Anim_Zombot_Mikey_ClawRT_Montage.Anim_Zombot_Mikey_ClawRT_Montage'"));
	if (AttackMontageRef.Object)
	{
		AttackMontage = AttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StaggerMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/DarkMetalPack/ZombotMike/Animations/Anim_Zombot_Mikey_HitRegister_Montage.Anim_Zombot_Mikey_HitRegister_Montage'"));
	if (StaggerMontageRef.Object)
	{
		StaggerMontage = StaggerMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> HPPotionMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Asset/Outline/M_Outline_Hp.M_Outline_Hp'"));
	if (HPPotionMaterialRef.Succeeded())
	{
		HPPotionMaterial = HPPotionMaterialRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MPPotionMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Asset/Outline/M_Outline_Mp.M_Outline_Mp'"));
	if (MPPotionMaterialRef.Succeeded())
	{
		MPPotionMaterial = MPPotionMaterialRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> AttackPotionMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Asset/Outline/M_Outline_Attack.M_Outline_Attack'"));
	if (AttackPotionMaterialRef.Succeeded())
	{
		AttackPotionMaterial = AttackPotionMaterialRef.Object;
	}
	
	AIControllerClass = AOVAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// HpBar = CreateDefaultSubobject<UOVWidgetComponent>(TEXT("Widget"));
	// HpBar->SetupAttachment(GetMesh());
	// HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	// static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UMG/WBP_HpBar.WBP_HpBar_C"));
	// if (HpBarWidgetRef.Class)
	// {
	// 	HpBar->SetWidgetClass(HpBarWidgetRef.Class);
	// 	HpBar->SetWidgetSpace(EWidgetSpace::World);
	// 	HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
	// 	HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// }

	DamageComponent = CreateDefaultSubobject<UOVDamageComponent>(TEXT("DamageComponent"));
	AttackComponent = CreateDefaultSubobject<UOVAttackComponent>(TEXT("AttackComponent"));
	Stat->SetMaxHp(50);
	DamageComponent->SetMaxHealth(50);
	bIsAttacking = false;
	
	//GetMesh()->SetOverlayMaterial(MPPotionMaterial);
	if(RandomItemName == "None")
	{
		UE_LOG(LogTemp,Warning, TEXT("NameNone"));
	}
}

void AOVCharacterNonPlayer::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 40.f;
	const float AttackRadius = 50.f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	//구체 만들어서 투사
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_OVACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageInfo DamageInfo = {20, E_DamageType::Melee, E_DamageResponses::HitReaction, false, false, false, false };
		IOVDamagableInterface* DamagableInterface = Cast<IOVDamagableInterface>(OutHitResult.GetActor());
		if (DamagableInterface)
		{
			DamagableInterface->TakeDamage(DamageInfo);
		}
	}

#if ENABLE_DRAW_DEBUG //디버그

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

void AOVCharacterNonPlayer::SetDead()
{
	Super::SetDead();
	//HpBar->SetHiddenInGame(true);
	PlayDeadAnimation();
	FTimerHandle DeadTimerHandle;
	AOVAIController* AIController = Cast<AOVAIController>(GetController());
	AIController->GetBrainComponent()->StopLogic(TEXT("Dead"));

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *RandomItemName);
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	AOVPickup* PotionObject = GetWorld()->SpawnActor<AOVPickup>(AOVPickup::StaticClass(), GetActorLocation(),
																GetActorRotation(), SpawnParams);
	PotionObject->DesiredItemID = *RandomItemName;
	PotionObject->InitialStart();

	
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AOVCharacterNonPlayer::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void AOVCharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();
	RandomItemName = GetRandomItemName();
}

void AOVCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	DamageComponent->OnDeath.AddUObject(this, &AOVCharacterNonPlayer::SetDead);
	DamageComponent->OnDamageResponse.AddUObject(this, &AOVCharacterNonPlayer::DamageResponse);
}

float AOVCharacterNonPlayer::GetCurrentHealth()
{
	return Stat->GetCurrentHp();
}

float AOVCharacterNonPlayer::GetMaxHealth()
{
	return Stat->GetMaxHp();
}

float AOVCharacterNonPlayer::Heal(float Amount)
{
	return -1;
}

bool AOVCharacterNonPlayer::TakeDamage(FDamageInfo DamageInfo)
{
	DamageComponent->TakeDamage(DamageInfo);
	Stat->SetHp(DamageComponent->Health);
	return true;
}

bool AOVCharacterNonPlayer::IsDead()
{
	return DamageComponent->bIsDead;
}

bool AOVCharacterNonPlayer::IsAttacking()
{
	return bIsAttacking;
}

void AOVCharacterNonPlayer::DamageResponse(E_DamageResponses DamageResponses)
{
	//GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(StaggerMontage, 1.0f);
	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &AOVCharacterNonPlayer::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(CompleteDelegate, StaggerMontage);
	//AnimInstance->Montage_SetEndDelegate(FOnMontageEnded::CreateUObject(this, &AOVCharacterNonPlayer::OnMontageEnded), StaggerMontage);
}

void AOVCharacterNonPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AOVGameMode* GameMode = Cast<AOVGameMode>(GetWorld()->GetAuthGameMode());
	AOVGameState* GameState = Cast<AOVGameState>(UGameplayStatics::GetGameState(GetWorld()));
	AOVAIController* AIController = Cast<AOVAIController>(GetController());
	if(GameMode->Battery == GameMode->GoalCount || GameState->GetCharacterDead())
	{
		
		AIController->GetBrainComponent()->StopLogic(TEXT("Dead"));
		AIController->StopAI();
	}
	if(GameState->bIsResumeMenuVisible)
	{
		AIController->GetBrainComponent()->PauseLogic(TEXT("Pause for UI"));
	}
	else if(!GameState->bIsResumeMenuVisible)
	{
		AIController->GetBrainComponent()->ResumeLogic(TEXT("Resume for UI"));
	}
	// APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	// if (PlayerController && HpBar)
	// {
	// 	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	// 	if (CameraManager)
	// 	{
	// 		FVector CameraLocation = CameraManager->GetCameraLocation();
 //            
	// 		// Get the location of the HpBar
	// 		FVector HpBarLocation = HpBar->GetComponentLocation();
 //            
	// 		// Calculate the direction from the HpBar to the camera
	// 		FVector Direction = CameraLocation - HpBarLocation;
	// 		FRotator LookAtRotation = Direction.Rotation();
 //            
	// 		// Set the relative rotation of the HpBar to face the camera
	// 		HpBar->SetWorldRotation(LookAtRotation);
	// 	}
	// }
}

void AOVCharacterNonPlayer::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

float AOVCharacterNonPlayer::GetAIPatrolRadius()
{
	return 300.f;
}

float AOVCharacterNonPlayer::GetAIDetectRange()
{
	return 800.f;
}

float AOVCharacterNonPlayer::GetAIAttackRange()
{
	return 100.f; // 공격 범위 정하자
}

float AOVCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AOVCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AOVCharacterNonPlayer::AttackByAI()
{
	//ProcessComboCommand();
	//1초뒤 공격 끝난다
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(AttackMontage, 1.0f);
	
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			NotifyActionEnd();
		}
	), 1.0f, false);
}

void AOVCharacterNonPlayer::NotifyActionEnd()
{
	Super::NotifyActionEnd();
	OnAttackFinished.ExecuteIfBound();
}

FString AOVCharacterNonPlayer::GetRandomItemName()
{
	// 0에서 2 사이의 랜덤 값을 생성
	int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, 2);

	// E_Item 열거형 값을 배열에 저장
	E_Item Items[] = { E_Item::HPPotion, E_Item::MPPotion, E_Item::AttackPotion };

	// 선택된 랜덤 값에 해당하는 E_Item 값을 반환
	E_Item SelectedItem = Items[RandomIndex];
	// 선택된 E_Item 값을 문자열로 변환
	switch (SelectedItem)
	{
	case E_Item::HPPotion:
		GetMesh()->SetOverlayMaterial(HPPotionMaterial);
		return TEXT("HPPotion");
	case E_Item::MPPotion:
		GetMesh()->SetOverlayMaterial(MPPotionMaterial);
		return TEXT("MPPotion");
	case E_Item::AttackPotion:
		GetMesh()->SetOverlayMaterial(AttackPotionMaterial);
		return TEXT("AttackPotion");
	default:
		return TEXT("None");
	}
}