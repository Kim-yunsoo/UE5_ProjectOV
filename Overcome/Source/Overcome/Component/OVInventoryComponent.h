// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OVInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UOVItemBase;

UENUM(BlueprintType)
enum class EItemAddResult: uint8 
{
	IAR_NoItemAdded UMETA(DisplayName="No item added"),
	IAR_PartialAmountItemAdd UMETA(DisplayName="Partial amount of item added"),
	IAR_AllItemAdded UMETA(DisplayName="All of item added"),
};

USTRUCT(BlueprintType)
struct FItemAddResult //인벤토리가 다 꽉찼는지 어떤 상태인지 ? 
{
	GENERATED_BODY()
	FItemAddResult():
	ActualAmountAdded(0),
	OperationResult(EItemAddResult::IAR_NoItemAdded),
	ResultMessage(FText::GetEmpty())
	{};

	//인벤토리에 추가할 때 실제적인 양
	UPROPERTY(BlueprintReadOnly, Category="Item Add Result")
	int32 ActualAmountAdded;
	//아이템을 추가했을떄의 인벤토리 상태 
	UPROPERTY(BlueprintReadOnly,Category="Item Add Result")
	EItemAddResult OperationResult;
	//상태를 알려주는 텍스트 문자
	UPROPERTY(BlueprintReadOnly,Category="Item Add Result")
	FText ResultMessage; //어떤 상태인지 알려주는 텍스트 상태

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded=0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	};
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded,const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded=PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdd;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	};
	static FItemAddResult AddedAll(const int32 AmountAdded,const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded=AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	};
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCOME_API UOVInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnInventoryUpdated OnInventoryUpdated;
	
	
	// Sets default values for this component's properties
	UOVInventoryComponent();
	UFUNCTION(Category="Inventory")
	FItemAddResult HandleAddItem(UOVItemBase* InputItem);
	UFUNCTION(Category="Inventory")
	UOVItemBase* FindMatchingItem(UOVItemBase* ItemIn) const;
	UFUNCTION(Category="Inventory")
	UOVItemBase* FindNextItemByID(UOVItemBase* ItemIn) const;
	UFUNCTION(Category="Inventory")
	UOVItemBase* FindNextPartialStack(UOVItemBase* ItemIn)const;
	UFUNCTION(Category="Inventory")
	void RemoveSingleInstanceOfItem(UOVItemBase* ItemToRemove);
	int32 RemoveAmountOfItem(UOVItemBase* ItemIn, int32 DesiredAmountToRemove);
	UFUNCTION(Category="Inventory")
	void SplitExistingStack(UOVItemBase* ItemIn, const int32 AmountToSplit);
	
	UFUNCTION(Category="Inventory")
	FORCEINLINE int32 GetSlotsCapacity() const {return InventorySlotsCapacity;}; //슬롯 용량
	UFUNCTION(Category="Inventory")
	FORCEINLINE TArray<UOVItemBase*> GetInventoryContents() const{return InventoryContents;}; //인벤토리 배열
	UFUNCTION(Category="Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity){InventorySlotsCapacity = NewSlotsCapacity ;}; // 슬롯 용량 설정
	
protected:
	// Called when the game starts
	UPROPERTY(EditInstanceOnly, Category= "Inventory")
	int32 InventorySlotsCapacity;
	UPROPERTY(VisibleAnywhere, Category= "Inventory")
	TArray<TObjectPtr<UOVItemBase>> InventoryContents; //인벤토리 
	
	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UOVItemBase* InputItem);
	int32 HandleStackableItems(UOVItemBase*ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UOVItemBase* StackableItem ,int32 InitialRequestedAddAmount);

	void AddNewItem(UOVItemBase* Item, const int32 AmountToAdd);
};