// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuPlayerController.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Widgets/MainMenu/MainMenuWidget.h"
#include "Data/DataStructures.h"

AMenuPlayerController::AMenuPlayerController()
{
	bShowMouseCursor = true;

	static ConstructorHelpers::FObjectFinder<UClass> MainMenuHUD_BP(TEXT("/Game/UI/MainMenu/MainMenuWidget_BP.MainMenuWidget_BP_C"));
	if (MainMenuHUD_BP.Object)
	{
		MenuWidgetClass = MainMenuHUD_BP.Object;
	}
}

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UMainMenuWidget>(this, MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
			MenuWidget->NavigateTo(EMenuLocation::MainMenu);
		}
	}
}