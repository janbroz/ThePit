// Fill out your copyright notice in the Description page of Project Settings.

#include "PitGameMode.h"
#include "Player/PitCharacter.h"
#include "Player/PitPlayerController.h"
#include "Player/PitPlayerState.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

APitGameMode::APitGameMode()
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

void APitGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerControllerList.Add(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("Hey there, a new player joined us!"));
}