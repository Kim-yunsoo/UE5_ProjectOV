// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVAIBossController.h"

#include "AI/OVAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Character/OVCharacterPlayer.h"
#include "GameFramework/Character.h"
#include "Interface/OVEnemyAIInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

AOVAIBossController::AOVAIBossController() 
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BehaviorTress/BB_Enemy_Base.BB_Enemy_Base'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BehaviorTress/BT_Enemy_Boss.BT_Enemy_Boss'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
	SetPerceptionSystem();
}

void AOVAIBossController::RunAI()
{
	bool RunResult = RunBehaviorTree(BTAsset);
	Boss->SetState(EAIState::Passive);
	ensure(RunResult);
}

void AOVAIBossController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}


void AOVAIBossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Boss = Cast<AOVAIBoss>(GetPawn());

	//Boss->SetState(E_AIState::Passive);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		CharacterPlayer = Cast<AOVCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (!CharacterPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("CharacterPlayer Wrong"));
		}
		
		UBlackboardComponent* BlackboardPtr = Blackboard.Get();
		if (UseBlackboard(BBAsset, BlackboardPtr))
		{
			Blackboard->SetValueAsObject(BBKEY_ATTACKTARGET, CharacterPlayer);
		}
		RunAI();
	}, 0.2f, false);
}

void AOVAIBossController::SetPerceptionSystem()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	if(AIPerception)
	{
		//시각
			UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
			SightConfig->SightRadius = 800;
			SightConfig->LoseSightRadius = 1200;
			SightConfig->PeripheralVisionAngleDegrees = 60;
			SightConfig->SetMaxAge(5.0);
			SightConfig->DetectionByAffiliation.bDetectEnemies = true;
			SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
			SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
			AIPerception->ConfigureSense(*SightConfig);


		//청각
			UAISenseConfig_Hearing* SoundConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SoundConfig"));
			SoundConfig->HearingRange = 500.f;
			SoundConfig->SetMaxAge(3.0);
			SoundConfig->DetectionByAffiliation.bDetectEnemies = true;
			SoundConfig->DetectionByAffiliation.bDetectNeutrals = true;
			SoundConfig->DetectionByAffiliation.bDetectFriendlies = true;
			AIPerception->ConfigureSense(*SoundConfig);
		
		// 데미지
			UAISenseConfig_Damage* DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
			DamageConfig->SetMaxAge(5.0);
			AIPerception->ConfigureSense(*DamageConfig);

		AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AOVAIBossController::HandlePerceptionUpdated);
	}
}

void AOVAIBossController::SetBlackBoardKey()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsEnum(BBKEY_STATE, static_cast<uint8>(Boss->GetState()));
	}
}

void AOVAIBossController::HandleSightSense(AActor* Actor, FAIStimulus Stimulus)
{
	if(!Stimulus.WasSuccessfullySensed())
		return;
	
	AOVCharacterPlayer* player = Cast<AOVCharacterPlayer>(Actor);
	if(!player)
		return;
	if ((Boss->GetState() == EAIState::Passive || Boss->GetState() == EAIState::Investigating)) // && player ) //&& (Actor == CharacterPlayer))
	{
		Boss->SetState(EAIState::Attacking);
		//UE_LOG(LogTemp, Warning, TEXT("Sight Attack"));
		SetBlackBoardKey();
	}
}

void AOVAIBossController::HandleSoundSense(AActor* Actor, FAIStimulus Stimulus)
{
	if((Boss->GetState() == EAIState::Passive || Boss->GetState() == EAIState::Investigating))
	{
		Boss->SetState(EAIState::Investigating);
		FVector Location = Stimulus.StimulusLocation;
		//UE_LOG(LogTemp, Warning, TEXT("Sound"));
		UBlackboardComponent* BlackboardPtr = Blackboard.Get();
		if (UseBlackboard(BBAsset, BlackboardPtr))
		{
			Blackboard->SetValueAsVector(BBKEY_POINTOFINTEREST, Location);
		}
		SetBlackBoardKey();
	}
}

void AOVAIBossController::HandleDamageSense(AActor* Actor, FAIStimulus Stimulus)
{
	//UE_LOG(LogTemp, Warning, TEXT("Damage"));
	if((Boss->GetState() == EAIState::Passive || Boss->GetState() == EAIState::Investigating))
	{
		Boss->SetState(EAIState::Attacking);
		SetBlackBoardKey();
	}
}

void AOVAIBossController::HandlePerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type.Name == "Default__AISense_Sight")
	{
		HandleSightSense(Actor, Stimulus);
	}
	if (Stimulus.Type.Name == "Default__AISense_Hearing")
	{
		HandleSoundSense(Actor, Stimulus);
	}
	if (Stimulus.Type.Name == "Default__AISense_Damage")
	{
		HandleDamageSense(Actor, Stimulus);
	}
}
