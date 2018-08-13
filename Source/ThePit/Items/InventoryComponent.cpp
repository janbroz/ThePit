// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "Items/Item.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInventoryComponent::InitSlots()
{
	for (int i = 0; i < 9; ++i) 
	{
		Inventory.Add(nullptr);
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Inventory);
	DOREPLIFETIME(UInventoryComponent, MaxSize);
	DOREPLIFETIME(UInventoryComponent, UsedSpace);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItemToInventory(class AItem* NewItem)
{
	if (UsedSpace < MaxSize)
	{
		for (auto& Slot : Inventory)
		{
			if (Slot == nullptr)
			{
				Slot = NewItem;
				UsedSpace++;
				NewItem->SetActorHiddenInGame(true);
				if (UpdateInventoryDelegate.IsBound())
				{
					UpdateInventoryDelegate.Broadcast();
				}
				break;
			}
		}
	}
}

void UInventoryComponent::OnRep_InventoryUpdated()
{
	if (UpdateInventoryDelegate.IsBound())
	{
		UpdateInventoryDelegate.Broadcast();
	}
}