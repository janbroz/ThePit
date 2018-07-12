// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PitAbilities/PitAttribute.h"
#include "Data/DataStructures.h"
#include "PitAbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEPIT_API UPitAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPitAbilityComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	UFUNCTION(BlueprintCallable)
		void InitiAttributeSet();

	FName GetNameFromClass();

	UFUNCTION()
		void OnRep_AttributeSetWasReplicated();
	UFUNCTION()
		void ChangeTestBool();
public:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributeSetWasReplicated)
		UPitAttribute* AttributeSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EHeroClass HeroClass;
	UPROPERTY(VisibleAnywhere, Replicated)
		bool TestBool2;
	
};
