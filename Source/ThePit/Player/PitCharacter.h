// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PitAbilities/PitAttribute.h"
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

	virtual void PossessedBy(AController* NewController) override;

	virtual void PostInitializeComponents() override;
	//virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

public:
	// Networking stuff.
	UFUNCTION(BlueprintCallable)
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(Reliable, Server, WithValidation)
		void Server_TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	
	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_UpdateEnemyHUDs();
	
	UFUNCTION()
		void OnRep_AbilitySystemReplicated();
	UFUNCTION()
		void OnRep_InventoryReplicated();
	UFUNCTION()
		void OnRep_BoolTest();

	UFUNCTION()
		void PickUpItem(class AItem* ItemToPick);
	UFUNCTION(Reliable, Server, WithValidation)
		void Server_PickUpItem(class AItem* ItemToPick);

	UFUNCTION()
		void Attack(FVector AttackLocation, EAttackType TypeOfAttack);
	UFUNCTION(Reliable, Server, WithValidation)
		void ServerAttack(FVector AttackLocation, EAttackType TypeOfAttack);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character information")
		FString HeroName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character information")
		TSoftObjectPtr<UTexture2D> IconAssetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character information")
		UTexture2D* PortraitIcon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character information")
		int32 Bla;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character information", Transient, ReplicatedUsing = OnRep_AbilitySystemReplicated)
		class UPitAbilityComponent* AbilitySystem;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character information", ReplicatedUsing = OnRep_InventoryReplicated)
		class UInventoryComponent* InventoryManager;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_BoolTest)
		bool BoolTest;
	/*UPROPERTY(Replicated)
		UPitAttribute* Attributes;*/

	class UCameraComponent* PlayerCamera;
	class USpringArmComponent* CameraArm;
};
