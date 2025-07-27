// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AStarComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "AStarCollabComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MSCDISS_API UAStarCollabComponent : public UActorComponent
{
	GENERATED_BODY()

	UAStarComponent* AStarComponent;

	USplineComponent* SplineComponent;
	
	UPROPERTY(EditAnywhere)
	AActor* SplineActor;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	
	
	// Sets default values for this component's properties
	UAStarCollabComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetupSpline();

	UFUNCTION(BlueprintCallable)
	void SetupComponent();
};
