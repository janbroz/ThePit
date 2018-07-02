// Fill out your copyright notice in the Description page of Project Settings.

#include "ThePitGameModeBase.h"
#include "Player/PitCharacter.h"
#include "Player/PitPlayerController.h"
#include "Player/PitPlayerState.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AThePitGameModeBase::AThePitGameModeBase()
{
	
	PlayerControllerClass = APitPlayerController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UClass> Player_BP(TEXT("/Game/Characters/PlayerCharacters/Jhon.Jhon_C"));
	if (Player_BP.Object)
	{
		DefaultPawnClass = Player_BP.Object;
	}
	else
	{
		DefaultPawnClass = APitCharacter::StaticClass();
	}

	PlayerStateClass = APitPlayerState::StaticClass();
	
}

