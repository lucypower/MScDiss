// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMPlayerPawnSpawner.h"

// Sets default values
AFSMPlayerPawnSpawner::AFSMPlayerPawnSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFSMPlayerPawnSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFSMPlayerPawnSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFSMPlayerPawnSpawner::SpawnPlayerPawn()
{
	FActorSpawnParameters spawnParameters;
	
	CurrentPawn = GetWorld()->SpawnActor<APawn>(PlayerPawn, FVector(0, 0, 0), FRotator(0, 0, 0), spawnParameters);
}

void AFSMPlayerPawnSpawner::DestroyPawn()
{
	CurrentPawn->Destroy();
}

