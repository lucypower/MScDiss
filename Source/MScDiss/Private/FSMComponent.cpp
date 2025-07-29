// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMComponent.h"

#include "FSMPlayerPawnComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
UFSMComponent::UFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WeaponNames = {"Sword", "Bow"};
	ItemNames = {"Potion", "Key", "Bomb"};
}

void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	UFSMPlayerPawnComponent::OnPlayerPawnSpawned.AddDynamic(this, &UFSMComponent::GetPlayerStats);

	CurrentState = EFSMState::Idle;
}

void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (CurrentState)
	{
		case EFSMState::GreetPlayer:

			DelayHandle.Invalidate();
			GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &UFSMComponent::SpeakToPlayer, 1, false);
			
		break;
		
		case EFSMState::Idle:

			
			break;

		case EFSMState::ResetShop:

			DelayHandle.Invalidate();
			GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &UFSMComponent::ResetShop, 2, false);

		break;

		case EFSMState::Evaluating:

			break;
		
		case EFSMState::DecidingItemsAndPrices:

			DelayHandle.Invalidate();
			GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &UFSMComponent::DecideShop, 2, false);

			break;

		case EFSMState::DecidingDiscounts:

			DelayHandle.Invalidate();
			GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &UFSMComponent::DecideDiscount, 2, false);
			
			break;
		
		case EFSMState::Displaying:

			DelayHandle.Invalidate();
			GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &UFSMComponent::DisplayShop, 2, false);

			break;
	}
}

void UFSMComponent::SpeakToPlayer()
{
	if (CurrentState == EFSMState::GreetPlayer)
	{
		if (ShopText == "Hello!")
		{
			ShopText = "Good morning young player!";		
		}
		else
		{
			ShopText = "Hello!";
		}
	}
	
	CurrentState = EFSMState::DecidingItemsAndPrices;	
}

void UFSMComponent::ResetShop()
{
	if (!ShopItemNames.IsEmpty())
	{
		ShopItemNames.Empty();
		ShopItemPrices.Empty();
	}

	ShopText = "";
	bAppliedDiscounts = false;
	
	CurrentState = EFSMState::DecidingItemsAndPrices;
}

void UFSMComponent::GetPlayerStats(APawn* Pawn)
{
	CurrentState = EFSMState::Evaluating;
	
	UFSMPlayerPawnComponent* Player = Pawn->GetComponentByClass<UFSMPlayerPawnComponent>();

	PlayerWeaponClass = Player->GetWeaponClass();
	PlayerCurrentGold = Player->GetCurrentGold();
	PlayerReputation = Player->GetReputation();

	CurrentState = EFSMState::ResetShop;
}

void UFSMComponent::DecideShop()
{
	ShopItemNames.Add(WeaponNames[PlayerWeaponClass]);

	for (int i = 0; i < 2; i++)
	{
		ShopItemNames.Add(ItemNames[FMath::RandRange(0, ItemNames.Num() - 1)]);
	}

	if (PlayerCurrentGold < 300)
	{
		ShopItemPrices.Add(FMath::RandRange(0, 350));

		for (int i = 0; i < 2; i++)
		{
			ShopItemPrices.Add(FMath::RandRange(0, 200));
		}
	}
	else if (300 <= PlayerCurrentGold && PlayerCurrentGold < 600)
	{
		ShopItemPrices.Add(FMath::RandRange(350, 700));

		for (int i = 0; i < 2; i++)
		{
			ShopItemPrices.Add(FMath::RandRange(50, 250));
		}
	}
	else if (600 <= PlayerCurrentGold && PlayerCurrentGold <= 1000)
	{
		ShopItemPrices.Add(FMath::RandRange(700, 1000));

		for (int i = 0; i < 2; i++)
		{
			ShopItemPrices.Add(FMath::RandRange(100, 300));
		}	
	}

	CurrentState = EFSMState::DecidingDiscounts;
}

void UFSMComponent::DecideDiscount()
{
	if (!bAppliedDiscounts && PlayerReputation != 5)
	{
		for (int i = 0; i < 3; i++)
		{
			if (FMath::RandRange(0, 5) == 1)
			{
				int DiscountMultiplier = 5 - PlayerReputation;
				float Discount = 1 + (DiscountMultiplier * 0.2);

				ShopItemPrices[i] = ShopItemPrices[i] * Discount;

				if (Discount > 1)
				{
					ShopText = "Reputation too low, shop prices increased!";
				}
				else
				{
					ShopText = "Good reputation! Discount Applied!";
				}
			}
		}

		bAppliedDiscounts = true;
	}

	CurrentState = EFSMState::Displaying;
}

void UFSMComponent::DisplayShop()
{
	CurrentState = EFSMState::Idle;
}

