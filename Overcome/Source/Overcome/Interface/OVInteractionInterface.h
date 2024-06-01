// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OVInteractionInterface.generated.h"

// This class does not need to be modified.

class AOVCharacterPlayer;

UENUM()
enum class EInteractableType : uint8
{
	Pickup UMETA(DisplayName = "Pickup"),
	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"), 
	Device UMETA(DisplayName = "Device"),
	Toggle UMETA(DisplayName = "Toggle"), //장치를 활성화하거나 다른 기능을 활성화
	Container UMETA(DisplayName = "Container")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_BODY()
	//상호작용 가능 유형 정보 구조체 생성자
	FInteractableData() :
	InteractableType(EInteractableType::Pickup),
	Name(FText::GetEmpty()),
	Action(FText::GetEmpty()),
	Quantity(0),
	InteractionDuration(0.0f)
	{
		
	};

	
	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;
	
	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	//pick up
	UPROPERTY(EditInstanceOnly)
	int32 Quantity;

	//door.. 인터렉트 시간 설정 
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
	
};
UINTERFACE(MinimalAPI)
class UOVInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OVERCOME_API IOVInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteract();
	virtual void EndInteract();
	virtual void Interact(AOVCharacterPlayer* PlayerCharacter);
	FInteractableData InteractableData;
};
