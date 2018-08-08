// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PitPlayerState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUIDelegate_OnUpdateScore);

UCLASS()
class THEPIT_API APitPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	APitPlayerState();
	
	UFUNCTION(BlueprintCallable)
		void SelectCharacter(TSubclassOf<class APitCharacter> Character);
	UFUNCTION(Reliable, Server, WithValidation)
		void ServerSelectCharacter(TSubclassOf<class APitCharacter> Character);
	UFUNCTION()
		void OnRep_CharacterSelected();
	UFUNCTION()
		void OnRep_ScoreModified();
	UFUNCTION()
		void OnRep_LevelModified();
	UFUNCTION(BlueprintCallable)
		bool HasCharacterSelected();

	UFUNCTION(Client, Reliable)
		void UpdateEnemyHUDs();
	UFUNCTION()
		void UpdatePlayerKills(int32 Amount);
	UFUNCTION()
		void UpdatePlayerDeaths(int32 Amount);
	UFUNCTION()
		void GainExperience(int Amount);
	UFUNCTION()
		void LevelUp();

public:

	UPROPERTY(BlueprintAssignable)
		FUIDelegate_OnUpdateScore UpdateScoreDelegate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerInformation, Replicated)
		TSubclassOf<class APitCharacter> SelectedHero;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = PlayerInformation)
		class APitCharacter* SelectedPawn;
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = PlayerInformation, Replicated)
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CharacterSelected, BlueprintReadOnly)
		uint32 bHasCharacterSelected : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo, ReplicatedUsing = OnRep_ScoreModified)
		int32 Kills;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo, ReplicatedUsing = OnRep_ScoreModified)
		int32 Deaths;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerInfo, Replicated)
		int32 CurrentExperience;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerInfo, Replicated)
		int32 NeededExperience;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerInfo, ReplicatedUsing = OnRep_LevelModified)
		int32 Level;

};
