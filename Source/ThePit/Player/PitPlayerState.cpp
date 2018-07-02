// Fill out your copyright notice in the Description page of Project Settings.

#include "PitPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Player/PitPlayerController.h"
#include "Player/PitCharacter.h"
#include "Engine/World.h"
#include "PitGameMode.h"

APitPlayerState::APitPlayerState()
{
	

}

void APitPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APitPlayerState, SelectedHero);
	DOREPLIFETIME(APitPlayerState, bHasCharacterSelected);
}

void APitPlayerState::SelectCharacter(TSubclassOf<class APitCharacter> Character)
{

	if (Role < ROLE_Authority)
	{
		ServerSelectCharacter(Character);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The something here"));
		if (!bHasCharacterSelected)
		{
			SelectedHero = Character;
			bHasCharacterSelected = true;

			APitPlayerController* PC = Cast<APitPlayerController>(GetOwner());
			if (PC)
			{
				UE_LOG(LogTemp, Warning, TEXT("The owner is a controller"));
				APitGameMode* Mode = Cast<APitGameMode>(GetWorld()->GetAuthGameMode());
				if (Mode)
				{
					Mode->DefaultPawnClass = Character;
					AActor* SpawnedActor = GetWorld()->SpawnActor(Character);
					if (SpawnedActor)
					{
						PC->SelectCharacter(Cast<APawn>(SpawnedActor));
						PC->SpawnPlayerHUD();
						UE_LOG(LogTemp, Warning, TEXT("The pawn was created"));
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("The owner is not a controller"));
			}

		}
	}
}

void APitPlayerState::ServerSelectCharacter_Implementation(TSubclassOf<class APitCharacter> Character)
{
	SelectCharacter(Character);
}

bool APitPlayerState::ServerSelectCharacter_Validate(TSubclassOf<class APitCharacter> Character)
{
	return true;
}

void APitPlayerState::OnRep_CharacterSelected()
{
	APitPlayerController* PC = Cast<APitPlayerController>(GetOwner());
	if (PC)
	{
		if (Role < ROLE_Authority)
		{
			PC->HiddeSelectionWidget();
			PC->SpawnPlayerHUD();
		}
	}
}