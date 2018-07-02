// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHUDWidget.h"
#include "Player/PitCharacter.h"



void UPlayerHUDWidget::SetupWidgetInformation(APawn* Character)
{
	APitCharacter* ValidChar = Cast<APitCharacter>(Character);
	if (ValidChar)
	{
		HeroPawn = ValidChar;
		UpdateHeroUI();
	}
}