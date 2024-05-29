// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_StopAttacking.h"

#include "AIController.h"
#include "AI/OVAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/OVCharacterPlayer.h"
#include "Interface/OVEnemyAIInterface.h"

UBTService_StopAttacking::UBTService_StopAttacking()
{
	NodeName = TEXT("StopAttackingIfTargetIsDead");
	Interval = 1.0f;
}

void UBTService_StopAttacking::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	IOVEnemyAIInterface* AIPawn = Cast<IOVEnemyAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return ;
	}
	
	AOVCharacterPlayer* TargetActor = Cast<AOVCharacterPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_ATTACKTARGET));
	if(TargetActor)
	{
		if(TargetActor->Stat->GetCurrentHp() == 0) //죽음 판정
		{
			//UE_LOG(LogTemp, Warning, TEXT("DEAD"));
			AIPawn->SetState(E_AIState::Passive);
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(BBKEY_STATE,static_cast<uint8>(AIPawn->GetState()));
		}
	}
}
