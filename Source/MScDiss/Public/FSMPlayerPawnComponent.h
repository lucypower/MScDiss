// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSMPlayerPawnComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerPawnSpawned, APawn*, Pawn);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MSCDISS_API UFSMPlayerPawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSMPlayerPawnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentGold;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Reputation;

	static FPlayerPawnSpawned OnPlayerPawnSpawned;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int GetWeaponClass() {return WeaponClass;};

	int GetCurrentGold() {return CurrentGold;};

	int GetReputation() {return Reputation;};
};
