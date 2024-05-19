// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OVCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Gun/OVGun.h"
#include "OVCharacterControlData.h"
#include "Interface/OVInteractionInterface.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Item/OVHpItemData.h"
#include "Skill/OVShieldSkill.h"
#include "Skill/OVTeleportSkill.h"
#include "Stat/OVCharacterStatComponent.h"
#include "UI/OVHUDWidget.h"
#include "UI/OVStatWidget.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogOVCharacter);

AOVCharacterPlayer::AOVCharacterPlayer()
{
	//Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.0, 80.0, 50.0);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_Jump.IA_OV_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ChangeControl.IA_OV_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ShoulderMove.IA_OV_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookXRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ShoulderLookX.IA_OV_ShoulderLookX'"));
	if (nullptr != InputActionShoulderLookXRef.Object)
	{
		ShoulderLookActionX = InputActionShoulderLookXRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookYRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ShoulderLookY.IA_OV_ShoulderLookY'"));
	if (nullptr != InputActionShoulderLookYRef.Object)
	{
		ShoulderLookActionY = InputActionShoulderLookYRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_QuaterMove.IA_OV_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AimActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_MouseRight.IA_OV_MouseRight'"));
	if (nullptr != AimActionRef.Object)
	{
		AimAction = AimActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShootActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_Shoot.IA_OV_Shoot'"));
	if (nullptr != ShootActionRef.Object)
	{
		ShootAction = ShootActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> WheelActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ChangeWeapon.IA_OV_ChangeWeapon'"));
	if (nullptr != WheelActionRef.Object)
	{
		WheelAction = WheelActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TeleportActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_Teleport.IA_OV_Teleport'"));
	if (nullptr != TeleportActionRef.Object)
	{
		TeleportAction = TeleportActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShieldActionRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_Shield.IA_OV_Shield'"));
	if (nullptr != ShieldActionRef.Object)
	{
		ShieldAction = ShieldActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InteractionActionRef(
	TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_Interact.IA_OV_Interact'"));
	if (nullptr != InteractionActionRef.Object)
	{
		InteractionAction = InteractionActionRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;
	bIsAiming = false;

	//Timeline
	SmoothCurveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineFront"));
	SmoothInterpFunction.BindUFunction(this, FName("SmoothInterpReturn"));
	SmoothTimelineFinish.BindUFunction(this, FName("SmoothOnFinish"));

	//Gun
	Gun = CreateDefaultSubobject<AOVGun>(TEXT("Gun"));
	bIsGun = true;

	//Item Action
	TakeItemActions.Add(
		FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AOVCharacterPlayer::DrinkHp)));
	TakeItemActions.Add(
		FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AOVCharacterPlayer::DrinkMp)));
	TakeItemActions.Add(
		FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AOVCharacterPlayer::DrinkAttack)));
	TakeItemActions.
		Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AOVCharacterPlayer::Damage)));

	//Skill
	TeleportSkillComponent = CreateDefaultSubobject<UOVTeleportSkill>(TEXT("TeleSkillComponent"));
	bIsActiveTeleportSkill = true;

	ShieldSkillComponent = CreateDefaultSubobject<UOVShieldSkill>(TEXT("ShieldSkillComponent"));
	bIsActiveShieldSkill = true;

	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 225.0f;
	BaseEyeHeight = 74.0f;
	TurningInPlace = ETurningPlaceType::ETIP_NotTurning;
}

void AOVCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Gun = GetWorld()->SpawnActor<AOVGun>(GunClass);
		Gun->SetOwner(this);
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Back_Socket"));
	}

	if (SmoothCurveFloat)
	{
		SmoothCurveTimeline->AddInterpFloat(SmoothCurveFloat, SmoothInterpFunction);
		SmoothCurveTimeline->SetTimelineFinishedFunc(SmoothTimelineFinish);
		SmoothCurveTimeline->SetLooping(false);
	}
	SetCharacterControl(CurrentCharacterControlType);
}

void AOVCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::Jumping);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this,
	                                   &AOVCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this,
	                                   &AOVCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookActionX, ETriggerEvent::Triggered, this,
	                                   &AOVCharacterPlayer::ShoulderLookX);
	EnhancedInputComponent->BindAction(ShoulderLookActionY, ETriggerEvent::Triggered, this,
	                                   &AOVCharacterPlayer::ShoulderLookY);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this,
	                                   &AOVCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::Aiming);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AOVCharacterPlayer::StopAiming);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::Shoot);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AOVCharacterPlayer::StopShoot);
	EnhancedInputComponent->BindAction(WheelAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::ChangeWeapon);
	EnhancedInputComponent->BindAction(TeleportAction, ETriggerEvent::Triggered, this,
	                                   &AOVCharacterPlayer::TeleportSkill);
	EnhancedInputComponent->BindAction(ShieldAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::ShieldSkill);
	EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::BeginInteract);
	//EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Completed, this, &AOVCharacterPlayer::EndInteract);
	//EndInteraction 안함
}

void AOVCharacterPlayer::SmoothInterpReturn(float Value)
{
	CameraBoom->TargetArmLength = (FMath::Lerp(250, 150, Value));
}

void AOVCharacterPlayer::SmoothOnFinish()
{
}


void AOVCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AOVCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	UOVCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	//��Ʈ�� ������ ������ �´�
	check(NewCharacterControl); //�ݵ�� �ִ��� Ȯ��

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	//��Ʈ�ѷ� ������ ���� CastChecked�� Ȯ��

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings(); //���� �ʱ�ȭ
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
	CurrentCharacterControlType = NewCharacterControlType;
}

void AOVCharacterPlayer::SetCharacterControlData(const UOVCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}


void AOVCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);

	ClearTurnInPlace(MovementVector.X);
	ClearTurnInPlace(MovementVector.Y);
}


void AOVCharacterPlayer::ShoulderLookX(const FInputActionValue& Value)
{
	float LookAxisVector = Value.Get<float>();
	AddControllerYawInput(LookAxisVector);
	//UE_LOG(LogTemp, Log, TEXT("LookX"));
	// if (bIsAiming && bIsGun)
	// 	TurnInPlace();
}

void AOVCharacterPlayer::ShoulderLookY(const FInputActionValue& Value)
{
	float LookAxisVector = Value.Get<float>();
	AddControllerPitchInput(LookAxisVector);
}

void AOVCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}
	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AOVCharacterPlayer::Aiming(const FInputActionValue& Value)
{
	if (bIsGun)
	{
		if (!bIsAiming)
		{
			bIsAiming = true;
			SmoothCurveTimeline->Play();
			bUseControllerRotationYaw = true;
			OnAimChanged.Broadcast(bIsAiming);
			ServerRPCAiming();
		}
		// else
		// {
		// 	StopAiming(Value);
		// }
	}
}

void AOVCharacterPlayer::StopAiming(const FInputActionValue& Value)
{
	bIsAiming = false;
	bUseControllerRotationYaw = false;
	SmoothCurveTimeline->Reverse();
	OnAimChanged.Broadcast(bIsAiming);
	ServerRPCStopAiming();
}


void AOVCharacterPlayer::Jumping(const FInputActionValue& Value)
{
	if (!bIsAiming)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

void AOVCharacterPlayer::ChangeWeapon(const FInputActionValue& Value)
{
	int GunSocket = Value.Get<float>();;
	if (GunSocket > 0)
	{
		bIsGun = true;
		ServerRPCIsGun(bIsGun);
	}
	else
	{
		bIsGun = false;
		ServerRPCIsGun(bIsGun);
	}
}

void AOVCharacterPlayer::AimOffset(float DeltaTime)
{
	//컨트롤러 회전 사용 중단
	if (!bIsAiming) return;
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) // 서있고 점프 안하는 상태
	{
		FRotator CurrentAimRoTation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotaion = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRoTation, StaringAimRotation);
		//회전값 가지고 오기
		AO_Yaw = DeltaAimRotaion.Yaw;
		if (TurningInPlace == ETurningPlaceType::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	if (Speed > 0.f || bIsInAir) // 달리거나 점프할 때
	{
		StaringAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningPlaceType::ETIP_NotTurning;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;

	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		//[270,360) -> [-90, 0]
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}


void AOVCharacterPlayer::TurnInPlace(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("AO_Yaw: %f"), AO_Yaw);
	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningPlaceType::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningPlaceType::ETIP_Left;
	}
	if (TurningInPlace != ETurningPlaceType::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.0f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;

		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningPlaceType::ETIP_NotTurning;
			StaringAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void AOVCharacterPlayer::PlayTurn(class UAnimMontage* MontagetoPlay, float PlayRate, float Duration)
{
	if (!bIsTurning)
	{
		bIsTurning = true;
		PlayAnimMontage(MontagetoPlay, PlayRate);
		// Declare the FTimerHandle within the function
		FTimerHandle TimerHandle;

		// Set up the timer to call the ResetTurning function after 0.2 seconds
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			this->bIsTurning = false;
		}, Duration, false);

		bIsTurning = false;
	}
}

void AOVCharacterPlayer::TurnRight90()
{
	PlayTurn(TurnRight_90, 1.5f, 0.5f);
}

void AOVCharacterPlayer::TurnLeft90()
{
	PlayTurn(TurnLeft_90, 1.5f, 0.5f);
}

void AOVCharacterPlayer::TurnRight180()
{
	PlayTurn(TurnRight_180, 1.7f, 0.6f);
}

void AOVCharacterPlayer::TurnLeft180()
{
	PlayTurn(TurnLeft_180, 1.7f, 0.6f);
}

void AOVCharacterPlayer::ClearTurnInPlace(float Force)
{
	if (Force != 0.0f)
	{
		ClearMotion();
	}
}

void AOVCharacterPlayer::ClearMotion()
{
	if (IsPlayingRootMotion())
	{
		StopAnimMontage(GetCurrentMontage());
	}
}

// void AOVCharacterPlayer::TurnInPlace()
// {
// 	
// 	float VelocityXY = GetCharacterMovement()->Velocity.Size2D();
// 	if (!(GetCharacterMovement()->IsFalling()) && !(VelocityXY > 0.0f))
// 	{
// 		FRotator DeltaRotation = GetActorRotation() - GetBaseAimRotation();
// 		DeltaRotation.Normalize();
// 		float DeltaYaw = DeltaRotation.Yaw * -1.0f;
//         //Todo 외적 사용해서 다시 구현하기 
// 		if ((DeltaYaw > 45.f) || (DeltaYaw < -45.f))
// 		{
// 			// if (DeltaYaw > 135.f)
// 			// 	TurnRight180();
// 			// else if (DeltaYaw < -135.f)
// 			// 	TurnLeft180();
// 			// else
// 			if(DeltaYaw > 45.f)
// 				TurnRight90();
// 			else if (DeltaYaw < -45.f)
// 				TurnLeft90();
// 		}
// 	}
// }

void AOVCharacterPlayer::Shoot()
{
	if (!bIsShooting)
	{
		bIsShooting = true;
		if (Gun->GetBulletCount())
		{
			Gun->PullTrigger();
			PlayAnimMontage(Shooting_Gun, 0.5);

			FTimerHandle TimerHandle;

			// Set up the timer to call the ResetTurning function after 0.2 seconds
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				bIsShooting = false;
			}, 0.5, false);
		}
	}
}

void AOVCharacterPlayer::StopShoot()
{
}

void AOVCharacterPlayer::ServerRPCStopAiming_Implementation()
{
	bIsAiming = false;
}

void AOVCharacterPlayer::ServerRPCIsGun_Implementation(bool IsGun)
{
	if (IsGun)
	{
		bIsGun = true;
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Rifle_Socket"));
	}
	else
	{
		bIsGun = false;
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Back_Socket"));
	}
}


void AOVCharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOVCharacterPlayer, bIsAiming);
	DOREPLIFETIME(AOVCharacterPlayer, bIsGun);
}


void AOVCharacterPlayer::TakeItem(UOVItemData* InItemData)
{
	if (InItemData)
	{
		//		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void AOVCharacterPlayer::DrinkHp(UOVItemData* InItemData)
{
	float HpIncreaseAmount = Stat->GetCurrentHp() + 30;
	Stat->SetHp(HpIncreaseAmount);
}

void AOVCharacterPlayer::DrinkMp(UOVItemData* InItemData)
{
	float MpIncreaseAmount = Stat->GetCurrentMp() + 20;
	Stat->SetMp(MpIncreaseAmount);
}

void AOVCharacterPlayer::DrinkAttack(UOVItemData* InItemData)
{
	float AttackIncreaseAmount = Stat->GetCurrentAttack() + 10;
	Stat->SetAttack(AttackIncreaseAmount);
}

void AOVCharacterPlayer::Damage(UOVItemData* InItemData)
{
	if (bIsActiveShieldSkill)
	{
		float HpIncreaseAmount = Stat->GetCurrentHp() - 30;
		Stat->SetHp(HpIncreaseAmount);
	}
}

void AOVCharacterPlayer::SetupHUDWidget(UOVHUDWidget* InUserWidget)
{
	OnAimChanged.AddUObject(InUserWidget, &UOVHUDWidget::UpdateTarget);
	UOVStatWidget* StatWidget = InUserWidget->StatWidget;
	if (StatWidget)
	{
		StatWidget->UpdateStatWidget(Stat->GetCurrentHp(), Stat->GetCurrentMp(), Stat->GetCurrentAttack());
		Stat->OnStatChanged.AddUObject(StatWidget, &UOVStatWidget::UpdateStatWidget);
	}
}

void AOVCharacterPlayer::TeleportSkill(const FInputActionValue& Value)
{
	if (bIsActiveTeleportSkill && Stat->GetCurrentMp())
	{
		bIsActiveTeleportSkill = false;
		TeleportSkillComponent->SkillAction();
		float MpIncreaseAmount = Stat->GetCurrentMp() - 20;
		Stat->SetMp(MpIncreaseAmount);
	}
}

void AOVCharacterPlayer::ShieldSkill(const FInputActionValue& Value)
{
	if (bIsActiveShieldSkill && Stat->GetCurrentMp())
	{
		bIsActiveShieldSkill = false;
		ShieldSkillComponent->SkillAction();
		float MpIncreaseAmount = Stat->GetCurrentMp() - 30;
		Stat->SetMp(MpIncreaseAmount);
	}
}

void AOVCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AimOffset(DeltaSeconds);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}
}

void AOVCharacterPlayer::PerformInteractionCheck()
{
	//todo
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{GetPawnViewLocation()}; //폰의 뷰 위치가 시작 자동으로 눈 높의를 나타내는 특정 값으로 초기화
	FVector TraceEnd{TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)}; //캐릭터 컨트롤러의 회전 값

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

	if (LookDirection > 0) //시선 벡터가 양수인 경우 앞을 보고 있다는 의미니까! 
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UOVInteractionInterface::StaticClass()))
			{
				//특정 인터페이스가 있는지 확인 
				//const float Distance = (TraceStart - TraceHit.ImpactPoint).Size();

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable )
				{
					FoundInteractable(TraceHit.GetActor()); // 상호작용 가능한 액터를 찾음
					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return; // 여전히 같은 것을 보고 있다
				}
			}
		}
		NoInteractableFound(); //아무것도 못찾은 경우 반환 
	}
}

void AOVCharacterPlayer::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting()) //타이머가 활성화 상태라면? 
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable) //상호작용하는 액터가 이미 있는 경우 
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}
	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	TargetInteractable->BeginFocus();
}

void AOVCharacterPlayer::NoInteractableFound()
{
	if (IsInteracting()) //타이머가 실행중이면
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction); // 타이머 초기화
	}

	if (InteractionData.CurrentInteractable) //상호작용 가능한 것이 있다면
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}
	}
	InteractionData.CurrentInteractable = nullptr;
	TargetInteractable = nullptr;
}

void AOVCharacterPlayer::BeginInteract()
{
	PerformInteractionCheck(); //보고 있다는 것을 확인

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();
		}

		if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
		{
			Interact();
		}
		else
		{
			GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
			                                this,
			                                &AOVCharacterPlayer::Interact,
			                                TargetInteractable->InteractableData.InteractionDuration,
			                                false);
		}
	}
}

void AOVCharacterPlayer::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction); // 타이머 초기화

	if(IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void AOVCharacterPlayer::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction); // 타이머 초기화

	if(IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}


void AOVCharacterPlayer::ServerRPCAiming_Implementation()
{
	bIsAiming = true;
}
