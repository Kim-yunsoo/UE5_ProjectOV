// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OVInventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EItemAddResult: uint8 
{
	IAR_NoItemAdded UMETA(DisplayName="No item added"),
	IAR_PartialAmountItemAdd UMETA(DisplayName="Partial amount of item added"),
	IAR_AllItemAdded UMETA(DisplayName="All of item added"),
};

USTRUCT()
struct FItemAddResult //인벤토리가 다 꽉찼는지 어떤 상태인지 ? 
{
	GENERATED_BODY()
	FItemAddResult();

	int32 ActualAmountAdded;
	EItemAddResult OperationResult;
	FText ResultMessage; //어떤 상태인지 알려주는 텍스트 상태 
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOME_API UOVInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOVInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
