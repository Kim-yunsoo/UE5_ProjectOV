// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "AI/OVAI.h"
#include "AIController.h"
#include "Interface/OVCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Physics/OVCollision.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IOVCharacterAIInterface* AIPawn = Cast<IOVCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange(); //800으로 설정

	TArray<FOverlapResult> OverlapResults; //오버랩 체크
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel( //멀티로 체크 -> 플레이어가 다수인경우 
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_OVACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);
	
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController()) //폰을 감지했는데 플레이어 캐릭터인 경우에만!
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_CHARACTER, Pawn);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_CHARACTER, nullptr);
}
