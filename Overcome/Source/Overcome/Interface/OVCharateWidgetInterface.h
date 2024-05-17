// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OVCharateWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOVCharateWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OVERCOME_API IOVCharateWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetupCharacterWidget(class UOVUserWidget* InUserWidget) = 0;
};
