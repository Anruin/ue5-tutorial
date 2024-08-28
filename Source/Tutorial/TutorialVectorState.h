// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutorialState.h"
#include "TutorialVector.h"
#include "TutorialVectorState.generated.h"

class ATutorialVector;

USTRUCT(BlueprintType)
struct FTutorialVectorStateData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Tutorial", meta=(MakeEditWidget=true))
	FVector StartLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Tutorial", meta=(MakeEditWidget=true))
	FVector FinishLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	FVector InitialScale = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	FVector TargetScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	float Scaling = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	float Delay = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	float Duration = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	FLinearColor StartColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	FLinearColor EndColor = FLinearColor::White;
};

USTRUCT(BlueprintType)
struct FTutorialVectorStateDataArray {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	FVector PlayerLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	FRotator PlayerRotation = FVector::DownVector.Rotation();

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	TArray<FTutorialVectorStateData> Array;
};

/**
 * 
 */
UCLASS()
class UTutorialVectorState : public UTutorialState {
	GENERATED_BODY()

public:
	virtual void Execute(AActor* WorldContext) override;
	virtual void Stop() override;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	TArray<AActor*> Actors;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	TSubclassOf<ATutorialVector> VectorActorClass = ATutorialVector::StaticClass();

	UPROPERTY(EditAnywhere, Category = "Tutorial")
	TArray<FTutorialVectorStateData> VectorData;
};
