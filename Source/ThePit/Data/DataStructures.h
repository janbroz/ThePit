// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataStructures.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class EMenuLocation : uint8
{
	MainMenu		UMETA(DisplayName = "MainMenu"),
	HostingOptions  UMETA(DisplayName = "HostingOptions"),
	ServerFinder	UMETA(DisplayName = "ServerFinder"),
	GameSetup		UMETA(DisplayName = "GameSetup"),
	Options			UMETA(DisplayName = "Options")
};

UENUM(BlueprintType)
enum class EPlayerLocation : uint8 
{
	MainMenu		UMETA(DisplayName = "MainMenu"),
	Lobby			UMETA(DisplayName = "Lobby"),
	Game			UMETA(DisplayValue = "Game")
};

UENUM(BlueprintType)
enum class EHeroClass : uint8
{
	Warrior			UMETA(DisplayName = "Warrior"),
	Mage			UMETA(DisplayName = "Mage"),
	Assasin			UMETA(DisplayName = "Assasin"),
	Ranger			UMETA(DisplayName = "Ranger")
};

USTRUCT(Blueprintable)
struct FHeroStats : public FTableRowBase
{
	GENERATED_BODY()
public:
	FHeroStats()
		: HealthDefault(0.f)
		, MaxHealthDefault(0.f)
		, ManaDefault(0.f)
		, MaxManaDefault(0.f)
		, StrengthDefault(0.f)
		, AgilityDefault(0.f)
		, IntellectDefault(0.f)
		, BaseArmorDefault(0.f)
		, ArmorDefault(0.f)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MinionStats)
		float HealthDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MinionStats)
		float MaxHealthDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MinionStats)
		float ManaDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MinionStats)
		float MaxManaDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MinionStats)
		float StrengthDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MinionStats)
		float AgilityDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MinionStats)
		float IntellectDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MinionStats)
		float BaseArmorDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MinionStats)
		float ArmorDefault;

};

class THEPIT_API DataStructures
{
public:
	DataStructures();
	~DataStructures();
};
