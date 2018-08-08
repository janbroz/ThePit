// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PitGameState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUIDelegate_OnUIScoreUpdate);

UCLASS()
class THEPIT_API APitGameState : public AGameState
{
	GENERATED_BODY()
public:
	APitGameState();
	
	void AddScore(int32 Points);

	void PlayerScoreKill(class APlayerState* Instigator);

	UFUNCTION(BlueprintCallable)
		void PlayerGainExperience(class APlayerState* PlayerInstigator, int32 Amount);

public:
	UPROPERTY(BlueprintAssignable)
		FUIDelegate_OnUIScoreUpdate UpdateScoreDelegate;

public:
	UPROPERTY(Replicated)
		int32 ReplyPoints;

};
