// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Widget.h"
#include "FSMComponent.generated.h"

UENUM(BlueprintType)
enum class EFSMState : uint8
{
	GreetPlayer,
	Idle,
	ResetShop,
	Evaluating,
	DecidingItemsAndPrices,
	DecidingDiscounts,
	Displaying
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MSCDISS_API UFSMComponent : public UActorComponent
{
	GENERATED_BODY()

	int PlayerWeaponClass, PlayerCurrentGold, PlayerReputation;
	
public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EFSMState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> ShopItemNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int> ShopItemPrices;

	TArray<FString> WeaponNames;
	TArray<FString> ItemNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString ShopText;

	FTimerHandle DelayHandle;
	bool bAppliedDiscounts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<bool> bDiscounts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Runs;

	double StartTime;
	double EndTime;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Sets default values for this component's properties
	UFSMComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SpeakToPlayer();

	UFUNCTION()
	void ResetShop();

	UFUNCTION(BlueprintCallable)
	void GetPlayerStats(APawn* Pawn);

	UFUNCTION()
	void DecideShop();

	UFUNCTION()
	void DecideDiscount();

	UFUNCTION()
	void DisplayShop();
};
