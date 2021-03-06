// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THEPIT_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMenuPlayerController();
	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	TSubclassOf<class UMainMenuWidget> MenuWidgetClass;
	class UMainMenuWidget* MenuWidget;
	TSubclassOf<class ULobbyWidget> LobbyWidgetClass;
	class ULobbyWidget* LobbyWidget;

	void EscapePressed();
};
