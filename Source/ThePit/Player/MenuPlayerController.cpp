// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuPlayerController.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Widgets/MainMenu/MainMenuWidget.h"
#include "Widgets/Lobby/LobbyWidget.h"
#include "PitGameInstance.h"
#include "Data/DataStructures.h"

AMenuPlayerController::AMenuPlayerController()
{
	bShowMouseCursor = true;

	static ConstructorHelpers::FObjectFinder<UClass> MainMenuHUD_BP(TEXT("/Game/UI/MainMenu/MainMenuWidget_BP.MainMenuWidget_BP_C"));
	if (MainMenuHUD_BP.Object)
	{
		MenuWidgetClass = MainMenuHUD_BP.Object;
	}

	static ConstructorHelpers::FObjectFinder<UClass> Lobby_BP(TEXT("/Game/UI/Lobby/Lobby_BP.Lobby_BP_C"));
	if (Lobby_BP.Object)
	{
		LobbyWidgetClass = Lobby_BP.Object;
	}
}

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UPitGameInstance* GameInstance = Cast<UPitGameInstance>(GetGameInstance());
	if (GameInstance && MenuWidgetClass && LobbyWidgetClass)
	{
		if (GameInstance->CurrentLocation == EPlayerLocation::MainMenu)
		{
			MenuWidget = CreateWidget<UMainMenuWidget>(this, MenuWidgetClass);
			if (MenuWidget && IsLocalController())
			{
				MenuWidget->AddToViewport();
				MenuWidget->NavigateTo(EMenuLocation::MainMenu);
			}
		}
		if (GameInstance->CurrentLocation == EPlayerLocation::Lobby)
		{
			LobbyWidget = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
			if (LobbyWidget && IsLocalController())
			{
				LobbyWidget->AddToViewport();
			}
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("We are at: %s"), GameInstance->CurrentLocation == EPlayerLocation::Lobby ? TEXT("Lobby") : TEXT("Menu")));
	}

	/*if (MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UMainMenuWidget>(this, MenuWidgetClass);
		if (MenuWidget && IsLocalController())
		{
			MenuWidget->AddToViewport();
			MenuWidget->NavigateTo(EMenuLocation::MainMenu);
		}
	}*/
}

void AMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("EscapePressed", IE_Pressed, this, &AMenuPlayerController::EscapePressed);
}

void AMenuPlayerController::EscapePressed()
{
	if (MenuWidget)
	{
		MenuWidget->NavigateTo(EMenuLocation::MainMenu);
	}
}