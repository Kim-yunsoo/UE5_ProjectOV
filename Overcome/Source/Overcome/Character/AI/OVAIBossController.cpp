// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/OVAIBossController.h"

#include "AI/OVAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Character/OVCharacterPlayer.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AOVAIBossController::AOVAIBossController() //Base
{
	// static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BehaviorTress/BB_Enemy_Base.BB_Enemy_Base'"));
	// if (nullptr != BBAssetRef.Object)
	// {
	// 	BBAsset = BBAssetRef.Object;
	// }
	//
	// static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BehaviorTress/BT_Enemy_Base.BT_Enemy_Base'"));
	// if (nullptr != BTAssetRef.Object)
	// {
	// 	BTAsset = BTAssetRef.Object;
	// }
}

void AOVAIBossController::RunAI()
{
	// UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	// if (UseBlackboard(BBAsset, BlackboardPtr))
	// {
	// 	//멀티로 한다면 이렇게 하면 안될 것 같음
	// 	AOVCharacterPlayer* CharacterPlayer = Cast<AOVCharacterPlayer>( UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	// 	if(!CharacterPlayer)
	// 		UE_LOG(LogTemp, Warning, TEXT("Wrong"));
	// 	Blackboard->SetValueAsObject(BBKEY_ATTACKTARGET, CharacterPlayer); 
	// 	bool RunResult = RunBehaviorTree(BTAsset);
	// 	ensure(RunResult);
	// }
}

void AOVAIBossController::OnPossess(APawn* InPawn)
{
	// Super::OnPossess(InPawn);
	// UE_LOG(LogTemp, Warning, TEXT("OnPossess"));
	//RunAI();
}
