// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSMPlayerPawnSpawner.generated.h"

UCLASS()
class MSCDISS_API AFSMPlayerPawnSpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APawn> PlayerPawn;

	APawn* CurrentPawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Sets default values for this actor's properties
	AFSMPlayerPawnSpawner();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnPlayerPawn();
	
	UFUNCTION(BlueprintCallable)
	void DestroyPawn();
};
