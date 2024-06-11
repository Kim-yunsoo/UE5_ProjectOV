// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OVCharacterPlayer.h"
#include "GameFramework/Actor.h"
#include "Interface/OVInteractionInterface.h"
#include "OVPickup.generated.h"

class UBoxComponent;
class UDataTable;
class UOVItemBase;

UCLASS()
class OVERCOME_API AOVPickup : public AActor ,public IOVInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties

	AOVPickup();

	void InitializePickup(const TSubclassOf<UOVItemBase> BaseClass, const int32 InQuantity);
	void InitializeDrop(UOVItemBase* ItemDrop, const int32 InQuantity);
	void InitialStart();
	FORCEINLINE UOVItemBase* GetItemData(){return ItemReference;}

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;
	UPrimitiveComponent* GetMeshComponent() const { return PickupMesh; }

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInterface> HPPotionMaterial;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInterface> MPPotionMaterial;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInterface> AttackPotionMaterial;
	

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	TObjectPtr<UOVItemBase> ItemReference;
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;
	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	UPROPERTY(EditAnywhere, Category = "Pickup|Trigger")
	TObjectPtr<UBoxComponent> Trigger;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Interact(AOVCharacterPlayer* PlayerCharacter) override;
	void UpdateInteractableData();


	void TakePickup(const AOVCharacterPlayer* Taker);

	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

// #if WITH_EDITOR
// 	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
// #endif
};
