// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PitAttribute.generated.h"

/**
 * Class for managing stats.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUIDelegate_OnUIShouldUpdate, float, Value);

USTRUCT(BlueprintType)
struct FAttributeData
{
	GENERATED_BODY()

	FAttributeData()
		: BaseValue(0.f)
		, CurrentValue(0.f)
	{}

	FAttributeData(float DefaultValue)
		: BaseValue(DefaultValue)
		, CurrentValue(DefaultValue)
	{}

	virtual ~FAttributeData()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute information")
		float BaseValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute information")
		float CurrentValue;
};

USTRUCT(BlueprintType)
struct FAttribute
{
	GENERATED_BODY()

public:
	FAttribute()
		: Attribute(nullptr)
		, AttributeOwner(nullptr)
	{}

	FAttribute(UProperty* NewProperty);
	static bool IsAttributeDataProperty(const UProperty* Property);
	UProperty* GetUProperty() const { return Attribute; }

public:
	UPROPERTY(EditAnywhere, Category = SunsetAttribute)
		UProperty* Attribute;
	UPROPERTY(VisibleAnywhere, Category = SunsetAttribute)
		UStruct* AttributeOwner;

};



UCLASS()
class THEPIT_API UPitAttribute : public UObject
{
	GENERATED_BODY()
public:
	UPitAttribute();
	~UPitAttribute();
	
public:
	virtual bool IsSupportedForNetworking() const override { return true; }

	UFUNCTION(BlueprintImplementableEvent, Category = "UI communication")
		void UpdateUI();

	UFUNCTION()
		void OnRep_HealthChanged();

	void ModifyAttribute();

public:
	UPROPERTY(BlueprintAssignable)
		FUIDelegate_OnUIShouldUpdate UpdateUIDelegate;


public:
	UPROPERTY(Replicated)
		float RepliFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor attributes", ReplicatedUsing = OnRep_HealthChanged)
		FAttributeData Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor attributes", Replicated)
		FAttributeData MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor attributes", Replicated)
		FAttributeData Mana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor attributes", Replicated)
		FAttributeData MaxMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor attributes", Replicated)
		FAttributeData Strength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor attributes", Replicated)
		FAttributeData Agility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor attributes", Replicated)
		FAttributeData Intellect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor attributes", Replicated)
		FAttributeData BaseArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor attributes", Replicated)
		FAttributeData Armor;
};
