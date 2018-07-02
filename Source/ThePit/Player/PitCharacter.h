// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PitCharacter.generated.h"

UCLASS()
class THEPIT_API APitCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APitCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character information")
		FString HeroName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character information")
		TSoftObjectPtr<UTexture2D> IconAssetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character information")
		UTexture2D* PortraitIcon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character information")
		int32 Bla;

	class UCameraComponent* PlayerCamera;
	class USpringArmComponent* CameraArm;
};
