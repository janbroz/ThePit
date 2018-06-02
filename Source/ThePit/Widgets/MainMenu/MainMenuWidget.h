// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/DataStructures.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEPIT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void NavigateTo(EMenuLocation Location);
};
