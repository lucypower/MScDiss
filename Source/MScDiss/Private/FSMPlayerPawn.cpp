// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMPlayerPawn.h"

// Sets default values
AFSMPlayerPawn::AFSMPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFSMPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	CurrentGold = FMath::RandRange(0, 1000);
	GoldSpent = FMath::RandRange(0, 1000);
	FloorsCompleted = FMath::RandRange(0, 10);
	BossesKilled = FMath::RandRange(0, 50);
	EnemiesKilled = FMath::RandRange(0, 500);
	ItemsPickedUp = FMath::RandRange(0, 50);
}

// Called every frame
void AFSMPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFSMPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

