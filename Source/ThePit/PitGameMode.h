// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PitGameMode.generated.h"

/**
 * 
 */
UCLASS()
class THEPIT_API APitGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	APitGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	int32 MaxNumPlayers;
	TArray<class APlayerController*> PlayerControllerList;
};
