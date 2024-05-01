// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OVCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "OVCharacterControlData.h"

AOVCharacterPlayer::AOVCharacterPlayer()
{
	//Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	// Input

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_Jump.IA_OV_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ChangeControl.IA_OV_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ShoulderMove.IA_OV_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ShoulderLook.IA_OV_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_QuaterMove.IA_OV_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AimActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_MouseRight.IA_OV_MouseRight'"));
	if (nullptr != AimActionRef.Object)
	{
		MouseRight = AimActionRef.Object;
	}


	CurrentCharacterControlType = ECharacterControlType::Shoulder;
	bIsAiming = false;
}

void AOVCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);
}

void AOVCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::Jumping);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::ChangeCharacterControl);
		EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::ShoulderMove);
		EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::ShoulderLook);
		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::QuaterMove);
		EnhancedInputComponent->BindAction(MouseRight, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::Aiming);
		EnhancedInputComponent->BindAction(MouseRight, ETriggerEvent::Completed, this, &AOVCharacterPlayer::StopAiming);
	
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
	UOVCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType]; //컨트롤 데이터 가지고 온다
	check(NewCharacterControl); //반드시 있는지 확인

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController()); //컨트롤러 가지고 오기 CastChecked로 확인

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings(); //매핑 초기화
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
}

void AOVCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
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
	bIsAiming = true;
}

void AOVCharacterPlayer::StopAiming(const FInputActionValue& Value)
{
	bIsAiming = false;
}

void AOVCharacterPlayer::Jumping(const FInputActionValue& Value)
{
	if (!bIsAiming)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}