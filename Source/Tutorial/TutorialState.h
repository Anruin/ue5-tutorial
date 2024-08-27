// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TutorialState.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UTutorialState : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, Category = "Tutorial")
	FVector PlayerLocation = FVector::ZeroVector;

	UPROPERTY(VisibleInstanceOnly, Category = "Tutorial")
	FRotator PlayerRotation = FRotator::ZeroRotator;

	virtual void Execute(AActor* WorldContext) {
	}

	virtual void Stop() {
	}
};
