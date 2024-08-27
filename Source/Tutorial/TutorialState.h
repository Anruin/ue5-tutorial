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
	virtual void Execute(AActor* WorldContext) {
	}

	virtual void Stop() {
	}
};
