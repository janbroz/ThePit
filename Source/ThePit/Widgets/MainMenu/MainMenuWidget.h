// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/DataStructures.h"
#include "PitGameInstance.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEPIT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Test")
		FUpdateUISessions StuffDelegate;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EMenuLocation MenuLocation;

public:	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void NavigateTo(EMenuLocation Location);

	UFUNCTION(BlueprintCallable)
		void AskForOnlineGames();
	UFUNCTION(BlueprintCallable)
		void OnSessionSearchFinished();
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateSessionsUI();

};
