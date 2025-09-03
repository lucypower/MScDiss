// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarCollabComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAStarCollabComponent::UAStarCollabComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAStarCollabComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UAStarCollabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAStarCollabComponent::SetupSpline()
{
	SplineComponent->ClearSplinePoints(true);

	SplineComponent->UpdateSpline();

	AStarComponent->AStar();
	TArray<FVector2D> SplinePoints = AStarComponent->GetPath();
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Spline Points: %d"), SplinePoints.Num()));

	for (int i = 0; i < SplinePoints.Num(); i++)
	{
		SplineComponent->AddSplinePoint(FVector(SplinePoints[i].X * 100, SplinePoints[i].Y * 100, 10), ESplineCoordinateSpace::World, true);
	}

	SplineComponent->UpdateSpline();

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Spline Setup");

}

void UAStarCollabComponent::SetupComponent()
{
	AActor* Owner = GetOwner();	
	AStarComponent = Owner->FindComponentByClass<UAStarComponent>();

	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), "Spline", AllActors);

	SplineActor = Cast<AActor>(AllActors[0]);
	SplineComponent = SplineActor->FindComponentByClass<USplineComponent>();
	
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Collab Comp Setup");

}

