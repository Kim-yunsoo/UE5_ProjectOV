// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OVCharacterBase.h"
#include "InputActionValue.h"
#include "OVCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API AOVCharacterPlayer : public AOVCharacterBase
{
	GENERATED_BODY()
	
public:
	AOVCharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Character Control Section
protected:
	void ChangeCharacterControl(); //V키 눌러서 컨트롤러 바꿨을 때 구현하는 함수
	void SetCharacterControl(ECharacterControlType NewCharacterControlType); //변경됐을 때 컨트롤 설정 바꾸는 함수
	virtual void SetCharacterControlData(const class UOVCharacterControlData* CharacterControlData) override;


//Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;


//Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MouseRight;//마우스 오른쪽

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MouseLeft;//마우스 오른쪽

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	void Aiming(const FInputActionValue& Value);
	void StopAiming(const FInputActionValue& Value);

	void Jumping(const FInputActionValue& Value);

	void Shooting(const FInputActionValue& Value);
	void StopShooting(const FInputActionValue& Value);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsAiming : 1; //조준 


public:
	const uint8 GetIsAiming() { return bIsAiming; };


	ECharacterControlType CurrentCharacterControlType;
};
