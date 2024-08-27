// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TutorialFsm.generated.h"

class UTutorialState;

UCLASS()
class ATutorialFsm : public AActor {
	GENERATED_BODY()

public:
	ATutorialFsm();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool IsFinished() const;
	virtual void Initialize();
	virtual void Execute();
	virtual void Stop();

	UPROPERTY(VisibleInstanceOnly, Category = "Tutorial")
	int32 CurrentTutorialStateIndex = -1;

	// FSM for the tutorial
	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	TArray<UTutorialState*> TutorialStates;
};
