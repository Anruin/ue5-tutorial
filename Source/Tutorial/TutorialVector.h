// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TutorialVector.generated.h"

class FTutorialTextMetadata;
class ATutorialSizeText;
class UArrowComponent;

UCLASS()
class ATutorialVector : public AActor {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	UArrowComponent* ArrowComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	FVector EndLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	FVector TargetScale = FVector::OneVector;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	FVector InitialScale = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	float Scaling = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	float Delay = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	float Duration = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	float ElapsedTime = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	FLinearColor StartColor = FLinearColor::White;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	FLinearColor EndColor = FLinearColor::White;

	UPROPERTY(VisibleInstanceOnly, Category = "Tutorial")
	ATutorialSizeText* SizeText = nullptr;

	ATutorialVector();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void StartAnimation(const FVector& InOrigin, const FVector& InEnd,
	                    const FVector& InInitialScale, const FVector& InTargetScale,
	                    const FLinearColor& InStartColor,
	                    const FLinearColor& InEndColor, float InDelay, float InDuration, float InScaling);

	TSharedPtr<FTutorialTextMetadata> GetSizeTextMetadata();
	void AssignSizeText(const TSharedPtr<FTutorialTextMetadata>& InSizeTextMetadata);
	void UpdateSizeText();
	void ReleaseSizeText();
};
