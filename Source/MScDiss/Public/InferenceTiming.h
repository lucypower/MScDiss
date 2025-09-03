// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InferenceTiming.generated.h"

/**
 * 
 */
UCLASS()
class MSCDISS_API UInferenceTiming : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Inference Timing")
	static double GetInferenceTime() {return FPlatformTime::Seconds();}
};
