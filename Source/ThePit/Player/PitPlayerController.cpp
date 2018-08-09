// Fill out your copyright notice in the Description page of Project Settings.

#include "PitPlayerController.h"
#include "Player/PitCharacter.h"
#include "Widgets/PlayerHUD/PlayerHUDWidget.h"
#include "Widgets/PlayerHUD/CharacterSelectionWidget.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "PitAbilities/PitAbilityComponent.h"
#include "Player/PitPlayerState.h"

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
	InputComponent->BindAction("TestInput", IE_Pressed, this, &APitPlayerController::TEST_INPUT);
	InputComponent->BindAction("Spacebar", IE_Pressed, this, &APitPlayerController::Test2);
}

void APitPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckMouseLocation();
}

void APitPlayerController::TEST_INPUT()
{
	if (Role < ROLE_Authority)
	{
		Server_TEST_INPUT();
	}
	else
	{
		APitCharacter* PitPawn = Cast<APitCharacter>(GetPawn());
		if (PitPawn)
		{
			//PitPawn->BoolTest = !PitPawn->BoolTest;
			UE_LOG(LogTemp, Warning, TEXT("Should change the test bool"));
			PitPawn->AbilitySystem->AttributeSet->ModifyAttribute();
			//PitPawn->Attributes->ModifyAttribute();
			PitPawn->AbilitySystem->ChangeTestBool();
			//PitPawn->BoolTest = !PitPawn->BoolTest;
		}
	}
}

void APitPlayerController::Test2()
{
	//if (Role < ROLE_Authority)
	//{
	//	Server_Test2();
	//}
	//else
	//{
	//	APitCharacter* PitPawn = Cast<APitCharacter>(GetPawn());
	//	if (PitPawn)
	//	{
	//		//FString Bla = PitPawn->AbilitySystem->TestBool2 ? "yes" : "no";
	//		FString Bla = FString::FromInt(PitPawn->AbilitySystem->AttributeSet->Health.CurrentValue);
	//		UE_LOG(LogTemp, Warning, TEXT("Server character health is: %s"), *Bla);
	//	}
	//}

	APitCharacter* PitPawn = Cast<APitCharacter>(GetPawn());
	if (PitPawn)
	{
		APitPlayerState* PitState = Cast<APitPlayerState>(PitPawn->PlayerState);
		if (PitState)
		{
			PitState->UpdateEnemyHUDs();
		}
	}
}

void APitPlayerController::Server_TEST_INPUT_Implementation()
{
	TEST_INPUT();
}

bool APitPlayerController::Server_TEST_INPUT_Validate()
{
	return true;
}

void APitPlayerController::Server_Test2_Implementation()
{
	Test2();
}

bool APitPlayerController::Server_Test2_Validate()
{
	return true;
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
	if (HUDWidget)
	{
		bShowingScore = !bShowingScore;
		HUDWidget->DisplayScore(bShowingScore);
	}
}

void APitPlayerController::LMBPressed()
{
	FHitResult Hit;

	GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, true, Hit);
	APitCharacter* SelfChar = Cast<APitCharacter>(GetPawn());
	if (Hit.bBlockingHit && SelfChar)
	{
		SelfChar->Attack(Hit.Location, EAttackType::PrimaryAttack);
	}
}

void APitPlayerController::RMBPressed()
{
	FHitResult Hit;

	GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, true, Hit);

	if (Hit.bBlockingHit)
	{
		APitCharacter* HitChar = Cast<APitCharacter>(Hit.GetActor());
		if (HitChar)
		{
			/*FString Bla = FString::FromInt(HitChar->AbilitySystem->AttributeSet->Strength.CurrentValue);
			UE_LOG(LogTemp, Warning, TEXT("Client character strength is: %s"), *Bla);*/

			APitPlayerState* PitState = Cast<APitPlayerState>(HitChar->PlayerState);
			if (PitState)
			{
				FString IsSomething = PitState->HasCharacterSelected() ? "True" : "False";

				UE_LOG(LogTemp, Warning, TEXT("Client player state says: %s"), *IsSomething);

			}
		}
	}


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

void APitPlayerController::AddPlayerEnemies()
{
	if (IsLocalController())
	{
		if (HUDWidget)
		{
			HUDWidget->AddPlayerEnemy();
		}
	}
}