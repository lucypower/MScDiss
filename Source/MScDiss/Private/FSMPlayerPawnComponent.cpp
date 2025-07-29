// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMPlayerPawnComponent.h"

FPlayerPawnSpawned UFSMPlayerPawnComponent::OnPlayerPawnSpawned;

// Sets default values for this component's properties
UFSMPlayerPawnComponent::UFSMPlayerPawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFSMPlayerPawnComponent::BeginPlay()
{
	Super::BeginPlay();

	WeaponClass = FMath::RandRange(0, 1);
	CurrentGold = FMath::RandRange(0, 1000);
	Reputation = FMath::RandRange(0, 10);

	APawn* Pawn = Cast<APawn>(GetOwner());
	UFSMPlayerPawnComponent::OnPlayerPawnSpawned.Broadcast(Pawn);
}


// Called every frame
void UFSMPlayerPawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

