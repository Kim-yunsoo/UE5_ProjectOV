// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVAIController.h"

#include "OVCharacterNonPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/OVAI.h"
#include "Character/OVCharacterPlayer.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"


AOVAIController::AOVAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BehaviorTress/BB_OVCharacter.BB_OVCharacter'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BehaviorTress/BT_OVCharacter.BT_OVCharacter'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
	SetPerceptionSystem();
}

void AOVAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation()); //홈포지션 설정
		
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AOVAIController::StopAI()
{
	//BrainComponent에 저장된다.
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AOVAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}

void AOVAIController::SetPerceptionSystem()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	if(AIPerception)
	{
		UAISenseConfig_Damage* DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
		DamageConfig->SetMaxAge(5.0);
		AIPerception->ConfigureSense(*DamageConfig);

		//AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AOVAIEnemyBaseController::HandleDamageSense);
		AIPerception->SetDominantSense(DamageConfig->GetSenseImplementation());

		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AOVAIController::HandleDamageSense);
	}
}

void AOVAIController::HandleDamageSense(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsObject(BBKEY_TARGET, Actor);
	}
	
	AOVCharacterNonPlayer * Nonplayer = Cast<AOVCharacterNonPlayer>(GetPawn());
}
