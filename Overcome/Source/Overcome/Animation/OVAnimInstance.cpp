// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/OVAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/OVCharacterPlayer.h"


UOVAnimInstance::UOVAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
	bIsAiming = false;

}

void UOVAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AOVCharacterPlayer>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UOVAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		bIsAiming = Owner->GetIsAiming();

		AO_Yaw = Owner->GetAO_Yaw();
		AO_Pitch =  Owner->GetAO_Pitch();
	}
}


void UOVAnimInstance::AnimNotify_PlayMontageNotify()
{
	UE_LOG(LogTemp, Log, TEXT("GrenadeCheck"))
}

AOVCharacterPlayer* UOVAnimInstance::GetOwner()
{
	return Owner;
}
