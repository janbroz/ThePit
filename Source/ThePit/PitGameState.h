// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PitGameState.generated.h"

/**
 * 
 */
UCLASS()
class THEPIT_API APitGameState : public AGameState
{
	GENERATED_BODY()
public:
	APitGameState();
	
	void AddScore(int32 Points);

public:
	UPROPERTY(Replicated)
		int32 ReplyPoints;

};
