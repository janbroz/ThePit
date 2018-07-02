// Fill out your copyright notice in the Description page of Project Settings.

#include "PitPlayerController.h"
#include "Player/PitCharacter.h"
#include "Widgets/PlayerHUD/PlayerHUDWidget.h"
#include "Widgets/PlayerHUD/CharacterSelectionWidget.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

APitPlayerController::APitPlayerController()
{
	bShowMouseCursor = true;

	static ConstructorHelpers::FObjectFinder<UClass> PlayerHUD_BP(TEXT("/Game/UI/PlayerHUD/PlayerHUD_BP.PlayerHUD_BP_C"));
	if (PlayerHUD_BP.Object)
	{
		HUDWidgetClass = PlayerHUD_BP.Object;
	}

	static ConstructorHelpers::FObjectFinder<UClass> Selection_BP(TEXT("/Game/UI/HeroSelection/Selection_BP.Selection_BP_C"));
	if (Selection_BP.Object)
	{
		SelectionWidgetClass = Selection_BP.Object;
	}
}

void APitPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (bHasCharacterSelected && IsLocalController())
	{
		if (HUDWidgetClass)
			HUDWidget = CreateWidget<UPlayerHUDWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
	else
	{
		if (SelectionWidgetClass && IsLocalController())
			SelectionWidget = CreateWidget<UCharacterSelectionWidget>(this, SelectionWidgetClass);
		if (SelectionWidget)
		{
			SelectionWidget->AddToViewport();
		}
	}
}

void APitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("VerticalMov", this, &APitPlayerController::VerticalMovement);
	InputComponent->BindAxis("HorizontalMov", this, &APitPlayerController::HorizontalMovement);

	InputComponent->BindAction("EscapePressed", IE_Pressed, this, &APitPlayerController::EscapePressed);
	InputComponent->BindAction("ScorePressed", IE_Pressed, this, &APitPlayerController::ScorePressed);
	InputComponent->BindAction("LMB", IE_Pressed, this, &APitPlayerController::LMBPressed);
	InputComponent->BindAction("RMB", IE_Pressed, this, &APitPlayerController::RMBPressed);
}

void APitPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckMouseLocation();
}

void APitPlayerController::SelectCharacter(APawn* NewPawn)
{
	SetPawn(NewPawn);
	Possess(NewPawn);

	if (IsLocalController())
	{
		SelectionWidget->RemoveFromParent();
	}

	if (Role < ROLE_Authority)
	{
		Server_SelectCharacter(NewPawn);
	}
}

void APitPlayerController::CheckMouseLocation()
{
	if (IsLocalController()) 
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
		if (Hit.bBlockingHit)
		{
			AlignCameraToMouse(Hit.Location);
		}
	}
}

void APitPlayerController::AlignCameraToMouse(FVector MouseLoc)
{
	ACharacter* PitChar = GetCharacter();
	if (!PitChar)
	{
		return;
	}

	FVector InitLoc = PitChar->GetActorLocation();
	FRotator LookAtRot = (MouseLoc - InitLoc).Rotation();
	FRotator MyRot = GetControlRotation();
	MyRot.Yaw = LookAtRot.Yaw;
	SetControlRotation(MyRot);

	if (Role < ROLE_Authority)
	{
		Server_AlignCameraToMouse(MouseLoc);
	}
}

void APitPlayerController::Server_AlignCameraToMouse_Implementation(FVector MouseLoc)
{
	AlignCameraToMouse(MouseLoc);
}

bool APitPlayerController::Server_AlignCameraToMouse_Validate(FVector MouseLoc)
{
	//UE_LOG(LogTemp, Warning, TEXT("At the server we had: %s"), *MouseLoc.ToString());
	return true;
}



void APitPlayerController::Server_SelectCharacter_Implementation(APawn* NewPawn)
{
	SelectCharacter(NewPawn);
}

bool APitPlayerController::Server_SelectCharacter_Validate(APawn* NewPawn)
{
	return true;
}

void APitPlayerController::VerticalMovement(float Amount)
{
	if (GetPawn() && Amount != 0.0f)
	{
		ACharacter* PitChar = GetCharacter();
		if (PitChar)
		{
			PitChar->AddMovementInput(FVector::ForwardVector, Amount);
		}
	}
}

void APitPlayerController::HorizontalMovement(float Amount)
{
	if (GetPawn() && Amount != 0.0f)
	{
		ACharacter* PitChar = GetCharacter();
		if (PitChar)
		{
			PitChar->AddMovementInput(FVector::RightVector, Amount);
		}
	}
}

void APitPlayerController::EscapePressed()
{
}

void APitPlayerController::ScorePressed()
{
}

void APitPlayerController::LMBPressed()
{
}

void APitPlayerController::RMBPressed()
{
}

bool APitPlayerController::HasCharacterSelected()
{
	return bHasCharacterSelected;
}

void APitPlayerController::HiddeSelectionWidget()
{
	if (SelectionWidget)
	{
		SelectionWidget->RemoveFromParent();
	}
}

void APitPlayerController::SpawnPlayerHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("Tried to spawn a hud"));
	if (IsLocalController())
	{

		if (HUDWidgetClass)
			HUDWidget = CreateWidget<UPlayerHUDWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->SetupWidgetInformation(GetPawn());
			HUDWidget->AddToViewport();
		}
	}
}