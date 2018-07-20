// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PitPlayerState.generated.h"

/**
 * 
 */
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
	UFUNCTION(BlueprintCallable)
		bool HasCharacterSelected();

	UFUNCTION(Client, Reliable)
		void UpdateEnemyHUDs();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerInformation, Replicated)
		TSubclassOf<class APitCharacter> SelectedHero;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = PlayerInformation)
		class APitCharacter* SelectedPawn;
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = PlayerInformation, Replicated)
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CharacterSelected, BlueprintReadOnly)
		uint32 bHasCharacterSelected : 1;

};
