// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEPIT_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Information")
		class APitCharacter* HeroPawn;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Information")
		TArray<class APitCharacter*> CurrentEnemies;
public:
	UFUNCTION()
		void SetupWidgetInformation(APawn* Character);
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHeroUI();
	UFUNCTION(BlueprintImplementableEvent)
		void AddPlayerEnemy();
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateEnemyHealthBars(class APitCharacter* Enemy);


};
