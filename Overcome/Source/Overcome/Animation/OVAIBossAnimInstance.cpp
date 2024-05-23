// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/OVAIBossAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UOVAIBossAnimInstance::UOVAIBossAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UOVAIBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
	//UE_LOG(LogTemp, Warning, TEXT("AISecond"));
}

void UOVAIBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Owner->GetActorRotation());
		FVector CurrentAccelaration =  Movement->GetCurrentAcceleration();
		if(CurrentAccelaration.Z != 0 && GroundSpeed > 3)
			ShouldMove = true;
		else
		{
			ShouldMove = false;
		}
	}
}
