// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVAIEnemyBaseController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/OVAI.h"
#include "Character/OVCharacterPlayer.h"
#include "Interface/OVEnemyAIInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"


AOVAIEnemyBaseController::AOVAIEnemyBaseController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BehaviorTress/BB_Enemy_Base.BB_Enemy_Base'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BehaviorTress/BT_Enemy_Base.BT_Enemy_Base'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}

	SetPerceptionSystem();
}

void AOVAIEnemyBaseController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		//멀티로 한다면 이렇게 하면 안될 것 같음
		//시간을 주자 액터 생명주기 확인
		CharacterPlayer = Cast<AOVCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		
		if(!CharacterPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("CharacterPlayer Wrong"));
		}
		Blackboard->SetValueAsObject(BBKEY_ATTACKTARGET, CharacterPlayer);
		Blackboard->SetValueAsEnum(BBKEY_STATE, static_cast<uint8>(AIState));

		// IOVEnemyAIInterface* AIPawn = Cast<IOVEnemyAIInterface>(this);
		// Blackboard->SetValueAsEnum(BBKEY_STATE, AIPawn->GetAIState());
		SetState(E_AIState::Passive);
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AOVAIEnemyBaseController::StopAI()
{
}

void AOVAIEnemyBaseController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//UE_LOG(LogTemp, Warning, TEXT("OnPossess"));
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		RunAI();
	}, 0.2f, false);
}

void AOVAIEnemyBaseController::SetPerceptionSystem()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	if(AIPerception)
	{
		//시각
		{
			UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
			SightConfig->SightRadius = 800;
			SightConfig->LoseSightRadius = 1200;
			SightConfig->PeripheralVisionAngleDegrees = 60;
			SightConfig->SetMaxAge(5.0);
			SightConfig->DetectionByAffiliation.bDetectEnemies = true;
			SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
			SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
			AIPerception->ConfigureSense(*SightConfig);

			//AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AOVAIEnemyBaseController::HandleSightSense);
		}

		//청각
		{
			UAISenseConfig_Hearing* SoundConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SoundConfig"));
			SoundConfig->HearingRange = 500.f;
			SoundConfig->SetMaxAge(3.0);
			SoundConfig->DetectionByAffiliation.bDetectEnemies = true;
			SoundConfig->DetectionByAffiliation.bDetectNeutrals = true;
			SoundConfig->DetectionByAffiliation.bDetectFriendlies = true;
			AIPerception->ConfigureSense(*SoundConfig);

			//AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AOVAIEnemyBaseController::HandleSoundSense);
		}

		// 데미지
		{
			UAISenseConfig_Damage* DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
			DamageConfig->SetMaxAge(5.0);
			AIPerception->ConfigureSense(*DamageConfig);

			//AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AOVAIEnemyBaseController::HandleDamageSense);
		}
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AOVAIEnemyBaseController::HandlePerceptionUpdated);
	}
}

void AOVAIEnemyBaseController::HandleSightSense(AActor* Actor, FAIStimulus Stimulus)
{
	if((AIState == E_AIState::Passive || AIState == E_AIState::Investigating) && (Actor == CharacterPlayer))
	{
		SetState(E_AIState::Attacking);
	}
	
}

void AOVAIEnemyBaseController::HandleSoundSense(AActor* Actor, FAIStimulus Stimulus)
{
	if((AIState == E_AIState::Passive || AIState == E_AIState::Investigating))
	{
		SetState(E_AIState::Investigating);
		FVector Location = Stimulus.StimulusLocation;
		UE_LOG(LogTemp, Warning, TEXT("Sound"));
		// UBlackboardComponent* BlackboardPtr = Blackboard.Get();
		// if (UseBlackboard(BBAsset, BlackboardPtr))
		{
			Blackboard->SetValueAsVector(BBKEY_POINTOFINTEREST, Location);
		}
	}
}

void AOVAIEnemyBaseController::HandleDamageSense(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Damage"));
	if((AIState == E_AIState::Passive || AIState == E_AIState::Investigating))
	{
		SetState(E_AIState::Attacking);
	}
}

void AOVAIEnemyBaseController::HandlePerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Stimulus.Type.Name.ToString());

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

void AOVAIEnemyBaseController::SetState(E_AIState AIStateValue)
{
	AIState = AIStateValue;
	//UE_LOG(LogTemp, Warning, TEXT("%hhd"), AIState);
	//Blackboard->SetValueAsEnum(BBKEY_STATE, static_cast<uint8>(AIState));
}

// E_AIState AOVAIEnemyBaseController::GetCurrentState()
// {
// 	return AIState;
// }
