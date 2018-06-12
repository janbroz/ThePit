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

public:
	// Replication and stuff
	UFUNCTION(Reliable, Server, WithValidation)
		void Server_AlignCameraToMouse(FVector MouseLoc);


public:
	TSubclassOf<class UPlayerHUDWidget> HUDWidgetClass;
	class UPlayerHUDWidget* HUDWidget;
};