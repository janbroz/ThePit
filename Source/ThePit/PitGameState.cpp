// Fill out your copyright notice in the Description page of Project Settings.

#include "PitGameState.h"
#include "UnrealNetwork.h"

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

