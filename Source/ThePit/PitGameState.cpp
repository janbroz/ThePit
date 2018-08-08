// Fill out your copyright notice in the Description page of Project Settings.

#include "PitGameState.h"
#include "UnrealNetwork.h"
#include "Player/PitPlayerState.h"

APitGameState::APitGameState()
{

}

void APitGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APitGameState, ReplyPoints);
}

void APitGameState::AddScore(int32 Points)
{
	ReplyPoints += Points;
}

void APitGameState::PlayerScoreKill(class APlayerState* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Tried to score a point"));
	for (auto &Player : PlayerArray)
	{
		auto SpecificPlayer = Cast<APitPlayerState>(Player);
		if (SpecificPlayer && SpecificPlayer == Cast<APitPlayerState>(Instigator))
		{
			SpecificPlayer->UpdatePlayerKills(1);
		}
	}

	if (UpdateScoreDelegate.IsBound())
	{
		UpdateScoreDelegate.Broadcast();
	}
}

void APitGameState::PlayerGainExperience(class APlayerState* PlayerInstigator, int32 Amount)
{
	APitPlayerState* Player = Cast<APitPlayerState>(PlayerInstigator);
	if (Player)
	{
		Player->GainExperience(Amount);
	}
}