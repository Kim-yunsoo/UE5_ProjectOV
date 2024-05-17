// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "OVWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
protected:
	virtual void InitWidget() override;
};
