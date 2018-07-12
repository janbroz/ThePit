// Fill out your copyright notice in the Description page of Project Settings.

#include "PitAbilityComponent.h"
#include "PitGameInstance.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

// Sets default values for this component's properties
UPitAbilityComponent::UPitAbilityComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	bWantsInitializeComponent = true;
	
	//SetIsReplicated(true);
	
	// ...
}


// Called when the game starts
void UPitAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	//SetIsReplicated(true);
	// ...	
}

void UPitAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPitAbilityComponent, AttributeSet);
	DOREPLIFETIME(UPitAbilityComponent, TestBool2);
}

bool UPitAbilityComponent::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (AttributeSet)
	{
		WroteSomething |= Channel->ReplicateSubobject(const_cast<UPitAttribute*>(AttributeSet), *Bunch, *RepFlags);
	}
	return WroteSomething;
}

// Called every frame
void UPitAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPitAbilityComponent::InitiAttributeSet()
{
	AttributeSet = NewObject<UPitAttribute>(GetOwner());
	
	UPitGameInstance* GameInstance = Cast<UPitGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		FName CName = GetNameFromClass();
		FHeroStats* HeroRowLookup = GameInstance->HeroStatsTable->FindRow<FHeroStats>(CName, "", true);
		if (HeroRowLookup)
		{
			AttributeSet->Strength = FAttributeData(HeroRowLookup->StrengthDefault);
			AttributeSet->Agility = FAttributeData(HeroRowLookup->AgilityDefault);
			AttributeSet->Intellect = FAttributeData(HeroRowLookup->IntellectDefault);
			AttributeSet->BaseArmor = FAttributeData(HeroRowLookup->BaseArmorDefault);
			AttributeSet->Armor = FAttributeData(HeroRowLookup->ArmorDefault);
			AttributeSet->MaxHealth = FAttributeData(HeroRowLookup->MaxHealthDefault + 2*HeroRowLookup->HealthDefault);
			AttributeSet->Health = AttributeSet->MaxHealth;
			AttributeSet->MaxMana = FAttributeData(HeroRowLookup->MaxManaDefault + 2* HeroRowLookup->ManaDefault);
			AttributeSet->Mana = AttributeSet->MaxMana;
		}
		
	}
}

FName UPitAbilityComponent::GetNameFromClass()
{
	switch (HeroClass)
	{
	case EHeroClass::Warrior:
		return FName("Warrior");
		break;
	case EHeroClass::Ranger:
		return FName("Ranger");
		break;
	case EHeroClass::Mage:
		return FName("Mage");
		break;
	case EHeroClass::Assasin:
		return FName("Assasin");
		break;
	default:
		return FName("Warrior");
		break;
	}
}

void UPitAbilityComponent::OnRep_AttributeSetWasReplicated()
{
	UE_LOG(LogTemp, Warning, TEXT("Replicated a variable from the server, yay"));
}

void UPitAbilityComponent::ChangeTestBool()
{
	TestBool2 = !TestBool2;
}