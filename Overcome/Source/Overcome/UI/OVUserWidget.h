// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OVUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCENOINLINE void SetOwningActor(AActor* NewOwner){OwningActor = NewOwner;}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwningActor;
};
