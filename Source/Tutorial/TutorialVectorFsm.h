// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutorialFsm.h"
#include "TutorialVectorState.h"
#include "TutorialVectorFsm.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ATutorialVectorFsm : public ATutorialFsm {
	GENERATED_BODY()

public:
	virtual void Initialize() override;

	UPROPERTY(EditInstanceOnly, Category = "Tutorial")
	TArray<FTutorialVectorStateDataArray> VectorData;
};
