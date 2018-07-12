// Fill out your copyright notice in the Description page of Project Settings.

#include "PitCharacter.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "PitAbilities/PitAbilityComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

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
	DOREPLIFETIME(APitCharacter, Attributes);
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

	/*if (HasAuthority())
	{
		AbilitySystem->AttributeSet = NewObject<UPitAttribute>(this);
		Attributes = NewObject<UPitAttribute>(this);
	}*/
}

//bool APitCharacter::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
//{
//	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
//	if (AbilitySystem->AttributeSet != nullptr)
//	{
//		WroteSomething |= Channel->ReplicateSubobject(AbilitySystem->AttributeSet, *Bunch, *RepFlags);
//	}
//	else {
//		UE_LOG(LogTemp, Warning, TEXT("Fuck it is empty"));
//	}
//	return WroteSomething;
//}