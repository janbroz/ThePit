// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "PitGameInstance.h"


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UPitGameInstance* GameInstance = GetWorld()->GetGameInstance<UPitGameInstance>();
	if (GameInstance)
	{
		GameInstance->Pit_OnUpdateSessionUI.AddDynamic(this, &UMainMenuWidget::OnSessionSearchFinished);
	}
}


void UMainMenuWidget::AskForOnlineGames()
{
	UPitGameInstance* GameInstance = GetWorld()->GetGameInstance<UPitGameInstance>();
	if (GameInstance)
	{
		GameInstance->FindOnlineGames();
	}
}

void UMainMenuWidget::OnSessionSearchFinished()
{
	//UPitGameInstance* GameInstance = GetWorld()->GetGameInstance<UPitGameInstance>();
	//if (GameInstance)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("There are exactly %d sessions"), GameInstance->BSearchResult.Num()));
	//	}

	//	//UE_LOG(LogTemp, Warning, TEXT("There are exactly %d sessions"), GameInstance->BSearchResult.Num());
	//}
	UpdateSessionsUI();
}