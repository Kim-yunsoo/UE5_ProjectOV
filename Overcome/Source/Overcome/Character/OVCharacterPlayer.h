// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OVCharacterBase.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "Interface/OVCharacterHUDInterface.h"
#include "Interface/OVCharacterItemInterface.h"
#include "Stat/OVCharacterStatComponent.h"
#include "TurningInPlace.h"
#include "OVCharacterPlayer.generated.h"

/**
 * 
 */

class UOVHUDWidget;
DECLARE_LOG_CATEGORY_EXTERN(LogOVCharacter, Log, All);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAimChangedDelegate, bool /*aim*/)

class AOVGun;

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UOVItemData* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate){}
	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class OVERCOME_API AOVCharacterPlayer : public AOVCharacterBase, public IOVCharacterItemInterface, public IOVCharacterHUDInterface
{
	GENERATED_BODY()

public:
	AOVCharacterPlayer();

protected:
	virtual void BeginPlay() override;
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Timeline Section
	FOnTimelineFloat SmoothInterpFunction; // (1)
	FOnTimelineEvent SmoothTimelineFinish; // (2)
	UFUNCTION()
	void SmoothInterpReturn(float Value); // (3)
	UFUNCTION()
	void SmoothOnFinish(); // (4)
	UPROPERTY()
	UTimelineComponent* SmoothCurveTimeline; // (5)
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* SmoothCurveFloat; // (6)


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
	TObjectPtr<class UInputAction> ShoulderLookActionX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookActionY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AimAction;//마우스 오른쪽

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WheelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TeleportAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShieldAction;
	
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLookX(const FInputActionValue& Value);
	void ShoulderLookY(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	void Aiming(const FInputActionValue& Value);
	void StopAiming(const FInputActionValue& Value);

	void Jumping(const FInputActionValue& Value);
	void ChangeWeapon(const FInputActionValue& Value);

	//AimOffset
	void AimOffset(float DeltaTime);

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StaringAimRotation;

public:
	FORCEINLINE float GetAO_Yaw() const {return AO_Yaw;};
	FORCEINLINE float GetAO_Pitch() const {return AO_Pitch;};

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Character")
	uint8 bIsAiming : 1; //조준 

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsGun : 1;

public:
	const uint8 GetIsAiming() { return bIsAiming; };
	ECharacterControlType CurrentCharacterControlType;

protected:
	ETurningPlaceType TurningInPlace;
	void TurnInPlace(float DeltaTime);
	
public:
	FORCEINLINE ETurningPlaceType GetTurningInPlace() const { return TurningInPlace; }
	
	//Gun
public:
	FORCEINLINE float GetAttack() {return Stat->GetCurrentAttack();}
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TSubclassOf<AOVGun> GunClass;
protected:
	void Shoot();
	void StopShoot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	uint8 bIsShooting : 1;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<class UAnimMontage> Shooting_Gun;

	UPROPERTY()
	AOVGun* Gun;

	// ServerRPC
	UFUNCTION(Server, Unreliable)
	void ServerRPCAiming();
	
	UFUNCTION(Server, Unreliable)
	void ServerRPCStopAiming();

	UFUNCTION(Server, Unreliable)
	void ServerRPCIsGun(bool IsGun);

	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void Tick(float DeltaSeconds) override;

	//Item
protected:
	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;
	
	virtual void TakeItem(UOVItemData* InItemData) override;
	void DrinkHp(class UOVItemData* InItemData);
	void DrinkMp(class UOVItemData* InItemData);
	void DrinkAttack(class UOVItemData* InItemData);
	void Damage(class UOVItemData* InItemData);

	//UI
public:
	FOnAimChangedDelegate OnAimChanged;
	
protected:
	virtual void SetupHUDWidget(UOVHUDWidget* InUserWidget) override;


	//Skill
public:
	UPROPERTY()
	uint8 bIsActiveTeleportSkill : 1;

	UPROPERTY()
	uint8 bIsActiveShieldSkill : 1;
	
	UPROPERTY()
	TObjectPtr<class UOVTeleportSkill> TeleportSkillComponent;

	UPROPERTY()
	TObjectPtr<class UOVShieldSkill> ShieldSkillComponent;
	
	void TeleportSkill(const FInputActionValue& Value);
	void ShieldSkill(const FInputActionValue& Value);

};