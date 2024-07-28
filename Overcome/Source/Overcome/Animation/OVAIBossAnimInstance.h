// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "OVAIBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVAIBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UOVAIBossAnimInstance();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner; //애님 인스턴스를 소유하고 있는 정보를 담는 변수 

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
	uint8 ShouldMove : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;
	
	virtual void NativeInitializeAnimation() override; //애님그래프 시작시 작동

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	/////////////////////////////////////////////////////////////////
	///
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Direction;
};
