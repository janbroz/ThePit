// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuGameMode.h"
#include "Player/MenuPawn.h"
#include "Player/MenuPlayerController.h"


AMenuGameMode::AMenuGameMode() 
{
	DefaultPawnClass = AMenuPawn::StaticClass();
	PlayerControllerClass = AMenuPlayerController::StaticClass();
}