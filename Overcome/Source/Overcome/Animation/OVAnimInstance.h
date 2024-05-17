// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TurningInPlace.h"
#include "OVAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UOVAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override; //애님그래프 시작시 작동

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //프레임마다 계속 업데이트 

	void AnimNotify_PlayMontageNotify();

	FORCEINLINE class AOVCharacterPlayer* GetOwner();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class AOVCharacterPlayer> Owner; //애님 인스턴스를 소유하고 있는 정보를 담는 변수 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1; // 사이즈가 명확한 bool 타입

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould; //움직이는지 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1; //떨어지고있는지

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1; //점프하고있는지

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould; //점프중인지 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsAiming : 1; //에임모드

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float AO_Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float AO_Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ETurningPlaceType TurningInPlace;
};
