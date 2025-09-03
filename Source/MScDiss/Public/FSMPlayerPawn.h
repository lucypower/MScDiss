// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FSMPlayerPawn.generated.h"

UCLASS()
class MSCDISS_API AFSMPlayerPawn : public APawn
{
	GENERATED_BODY()

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentGold;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int GoldSpent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int FloorsCompleted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int BossesKilled;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int EnemiesKilled;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int ItemsPickedUp;

	
	// Sets default values for this pawn's properties
	AFSMPlayerPawn();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
