// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PitPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THEPIT_API APitPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	APitPlayerController();
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
public:
	void VerticalMovement(float Amount);
	void HorizontalMovement(float Amount);

	void EscapePressed();
	void ScorePressed();
	void LMBPressed();
	void RMBPressed();

	void AlignCameraToMouse(FVector MouseLoc);
	void CheckMouseLocation();

	bool HasCharacterSelected();
	void SelectCharacter(APawn* NewPawn);
	void HiddeSelectionWidget();
	void SpawnPlayerHUD();
	void AddPlayerEnemies();

	void ShowPlayerInventory();

	void TEST_INPUT();
	void Test2();

public:
	// Replication and stuff
	UFUNCTION(Reliable, Server, WithValidation)
		void Server_AlignCameraToMouse(FVector MouseLoc);
	UFUNCTION(Reliable, Server, WithValidation)
		void Server_SelectCharacter(APawn* NewPawn);
	UFUNCTION(Reliable, Server, WithValidation)
		void Server_TEST_INPUT();
	UFUNCTION(Reliable, Server, WithValidation)
		void Server_Test2();

public:
	// Widgets
	TSubclassOf<class UPlayerHUDWidget> HUDWidgetClass;
	class UPlayerHUDWidget* HUDWidget;

	TSubclassOf<class UCharacterSelectionWidget> SelectionWidgetClass;
	class UCharacterSelectionWidget* SelectionWidget;

	// Information
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player information")
		class APitCharacter* SelectedEnemy;


private:
	uint32 bHasCharacterSelected : 1;
	uint32 bShowingScore : 1;
	uint32 bShowingInventory : 1;

};
