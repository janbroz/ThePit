// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUIDelegate_OnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEPIT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
		void AddItemToInventory(class AItem* NewItem);
	UFUNCTION()
		void InitSlots();
	UFUNCTION()
		void OnRep_InventoryUpdated();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_InventoryUpdated)
		TArray<class AItem*> Inventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		int32 MaxSize = 9;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
		int32 UsedSpace = 0;

	UPROPERTY(BlueprintAssignable)
		FUIDelegate_OnInventoryUpdated UpdateInventoryDelegate;
};
