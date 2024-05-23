// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_IsWieldingSword.h"

#include "AIController.h"
#include "Character/AI/OVEnemy_Base.h"

UBTDecorator_IsWieldingSword::UBTDecorator_IsWieldingSword()
{
	NodeName = TEXT("IsWieldingSword");
}

bool UBTDecorator_IsWieldingSword::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	AOVEnemy_Base * Enemy_Base = Cast<AOVEnemy_Base>(ControllingPawn);
	if(Enemy_Base->bIsWieldingWeapon)
	{
		return true;
	}
	else
	{
		return false;
	}
}
