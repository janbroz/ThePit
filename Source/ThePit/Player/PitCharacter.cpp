// Fill out your copyright notice in the Description page of Project Settings.

#include "PitCharacter.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "PitAbilities/PitAbilityComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Player/PitPlayerState.h"
#include "PitGameState.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
APitCharacter::APitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 1000.f;
	CameraArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraArm->bDoCollisionTest = false;
	CameraArm->bAbsoluteRotation = true;
	CameraArm->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetupAttachment(CameraArm, CameraArm->SocketName);

	// Abilities and stuff
	AbilitySystem = CreateDefaultSubobject<UPitAbilityComponent>(TEXT("Ability system"));
	if (AbilitySystem)
	{
		AbilitySystem->SetNetAddressable();
		AbilitySystem->SetIsReplicated(true);
	}
	bReplicates = true;
	
}

// Called when the game starts or when spawned
void APitCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	/*if (AbilitySystem)
	{
		
		AbilitySystem->InitiAttributeSet();
	}*/
}

// Called every frame
void APitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APitCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APitCharacter, AbilitySystem);
	DOREPLIFETIME(APitCharacter, BoolTest);
	/*DOREPLIFETIME(APitCharacter, Attributes);*/
}

void APitCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	Multicast_UpdateEnemyHUDs();
}

void APitCharacter::Multicast_UpdateEnemyHUDs_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("The player was possesed by an evil ancient"));
	
	APitPlayerState* PState = Cast<APitPlayerState>(this->PlayerState);
	if (PState)
	{
		PState->UpdateEnemyHUDs();
	}
}

void APitCharacter::OnRep_AbilitySystemReplicated()
{
	UE_LOG(LogTemp, Warning, TEXT("Shit, there was a change on the fucking ability system"));
}

void APitCharacter::OnRep_BoolTest()
{
	UE_LOG(LogTemp, Warning, TEXT("Bool test changed"));
}

void APitCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		AbilitySystem->SetIsReplicated(true);
		AbilitySystem->InitiAttributeSet();
	}
}

float APitCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float NewDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (Role < ROLE_Authority)
	{
		Server_TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		AbilitySystem->AttributeSet->ModifyAttribute();
		if (AbilitySystem->AttributeSet->Health.CurrentValue <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("It should give the player a point"));
			APitGameState* GState = Cast<APitGameState>(GetWorld()->GetGameState());
			if (GState)
			{
				GState->PlayerScoreKill(PlayerState);
			}
		}
	}

	

	return NewDamage;
}

void APitCharacter::Server_TakeDamage_Implementation(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

bool APitCharacter::Server_TakeDamage_Validate(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return true;
}

/*	This is where we should do a lot of stuff to check how is our pawn going
	to attack. Is it melee? Ranged? A Spell? Lets figure it!
*/
void APitCharacter::Attack(FVector AttackLocation, EAttackType TypeOfAttack)
{
	DrawDebugLine(GetWorld(), GetActorLocation(), AttackLocation, FColor::Red, false, 2.f, 1, 2.f);


	if (Role < ROLE_Authority)
	{
		ServerAttack(AttackLocation, TypeOfAttack);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("This was a valid attack"));
		
		
		/*APitCharacter* CharacterTarget = Cast<APitCharacter>(Target);
		if (CharacterTarget)
		{
			CharacterTarget->AbilitySystem->AttributeSet->ModifyAttribute();

		}*/
	}
}

void APitCharacter::ServerAttack_Implementation(FVector AttackLocation, EAttackType TypeOfAttack)
{
	Attack(AttackLocation, TypeOfAttack);
}

bool APitCharacter::ServerAttack_Validate(FVector AttackLocation, EAttackType TypeOfAttack)
{
	return true;
}