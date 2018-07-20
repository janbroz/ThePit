// Fill out your copyright notice in the Description page of Project Settings.

#include "PitPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Player/PitPlayerController.h"
#include "Player/PitCharacter.h"
#include "Engine/World.h"
#include "PitGameMode.h"
#include "GameFramework/GameState.h"

APitPlayerState::APitPlayerState()
{
	bAlwaysRelevant = true;

}

void APitPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APitPlayerState, SelectedHero);
	DOREPLIFETIME(APitPlayerState, bHasCharacterSelected);
	DOREPLIFETIME(APitPlayerState, SelectedPawn);
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
				AActor* SpawnedActor = GetWorld()->SpawnActor(Character);
				if (SpawnedActor)
				{
					SelectedPawn = Cast<APitCharacter>(SpawnedActor);
					PC->SelectCharacter(Cast<APawn>(SpawnedActor));
					PC->SpawnPlayerHUD();
					UpdateEnemyHUDs();
				}

				//APitGameMode* Mode = Cast<APitGameMode>(GetWorld()->GetAuthGameMode());
				//if (Mode)
				//{
				//	Mode->DefaultPawnClass = Character;
				//	AActor* SpawnedActor = GetWorld()->SpawnActor(Character);
				//	if (SpawnedActor)
				//	{
				//		SelectedPawn = Cast<APitCharacter>(SpawnedActor);
				//		PC->SelectCharacter(Cast<APawn>(SpawnedActor));
				//		PC->SpawnPlayerHUD();
				//		//PC->AddPlayerEnemies();
				//		//UE_LOG(LogTemp, Warning, TEXT("The pawn was created"));
				//	}
				//}

				for (auto& PlayerS : GetWorld()->GetGameState()->PlayerArray) 
				{
					APitPlayerState* PitPlayerS = Cast<APitPlayerState>(PlayerS);
					if (PitPlayerS && PitPlayerS->HasCharacterSelected())
					{
						PitPlayerS->UpdateEnemyHUDs();
					}
				}
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
	if (PC && PC->IsLocalPlayerController())
	{
		PC->HiddeSelectionWidget();
		PC->SpawnPlayerHUD();
		UpdateEnemyHUDs();
	}


	//APitPlayerController* PC = Cast<APitPlayerController>(GetOwner());
	//if (PC)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Someone picked something"));
	//	if (Role < ROLE_Authority)
	//	{
	//		PC->HiddeSelectionWidget();
	//		PC->SpawnPlayerHUD();
	//		//PC->AddPlayerEnemies();
	//		//UpdateEnemyHUDs();

	//		for (auto& PlayerS : GetWorld()->GetGameState()->PlayerArray)
	//		{
	//			APitPlayerState* PitPlayerS = Cast<APitPlayerState>(PlayerS);
	//			if (PitPlayerS && PitPlayerS->HasCharacterSelected())
	//			{
	//				PitPlayerS->UpdateEnemyHUDs();
	//			}
	//		}
	//	}
	//}

	//UpdateEnemyHUDs();
}

void APitPlayerState::UpdateEnemyHUDs_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Player is: %s"), *GetPlayerName());
	APitPlayerController* PC = Cast<APitPlayerController>(GetOwner());
	if (PC)
	{
		if (PC->IsLocalController())
		{
			PC->AddPlayerEnemies();
		}
	}
}

bool APitPlayerState::HasCharacterSelected()
{
	return bHasCharacterSelected;
}