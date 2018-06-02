// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructures.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class EMenuLocation : uint8
{
	MainMenu		UMETA(DisplayName = "MainMenu"),
	ServerFinder	UMETA(DisplayName = "ServerFinder"),
	Options			UMETA(DisplayName = "Options")
};


class THEPIT_API DataStructures
{
public:
	DataStructures();
	~DataStructures();
};
