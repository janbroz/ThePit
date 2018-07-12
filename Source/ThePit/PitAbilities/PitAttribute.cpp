// Fill out your copyright notice in the Description page of Project Settings.

#include "PitAttribute.h"
#include "Net/UnrealNetwork.h"

UPitAttribute::UPitAttribute()
{
	
}

UPitAttribute::~UPitAttribute()
{

}

void UPitAttribute::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPitAttribute, Health);
	DOREPLIFETIME(UPitAttribute, MaxHealth);
	DOREPLIFETIME(UPitAttribute, Mana);
	DOREPLIFETIME(UPitAttribute, MaxMana);
	DOREPLIFETIME(UPitAttribute, Strength);
	DOREPLIFETIME(UPitAttribute, Agility);
	DOREPLIFETIME(UPitAttribute, Intellect);
	DOREPLIFETIME(UPitAttribute, BaseArmor);
	DOREPLIFETIME(UPitAttribute, Armor);
}


FAttribute::FAttribute(UProperty* NewProperty)
{
	Attribute = nullptr;
	AttributeOwner = nullptr;

	if (IsAttributeDataProperty(NewProperty))
	{
		Attribute = NewProperty;
	}
}

bool FAttribute::IsAttributeDataProperty(const UProperty* Property)
{
	const UStructProperty* StructProp = Cast<UStructProperty>(Property);
	if (StructProp)
	{
		const UStruct* Struct = StructProp->Struct;
		if (Struct && Struct->IsChildOf(FAttributeData::StaticStruct()))
		{
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Awkward, It is not a valid property"));
		}
	}
	return false;
}

void UPitAttribute::ModifyAttribute()
{
	Health.CurrentValue -= 1.f;
	RepliFloat += 2.f;

	UE_LOG(LogTemp, Warning, TEXT("Called on server?"));
	if (UpdateUIDelegate.IsBound())
	{
		UpdateUIDelegate.Broadcast(FMath::Rand());
	}
}

void UPitAttribute::OnRep_HealthChanged()
{
	UE_LOG(LogTemp, Warning, TEXT("the health was modified"));
	if (UpdateUIDelegate.IsBound())
	{
		UpdateUIDelegate.Broadcast(FMath::Rand());
	}
}