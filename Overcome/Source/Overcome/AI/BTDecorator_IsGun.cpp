// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsGun.h"

#include "AIController.h"
#include "OVAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/OVCharacterPlayer.h"

UBTDecorator_IsGun::UBTDecorator_IsGun()
{
	NodeName = "Is Player Gun";
}

bool UBTDecorator_IsGun::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		APawn* AIPawn = AIController->GetPawn();
		if (AIPawn)
		{
			UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_CHARACTER);
			AOVCharacterPlayer* TargetActor = Cast<AOVCharacterPlayer>(TargetObject);

			if (TargetActor)
			{
				return TargetActor->GetIsGun();
			}
		}
	}
	return false;
}
