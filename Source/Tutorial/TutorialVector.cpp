// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialVector.h"

#include "Components/ArrowComponent.h"

ATutorialVector::ATutorialVector() {
	PrimaryActorTick.bCanEverTick = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetHiddenInGame(false);
	ArrowComponent->SetVisibility(true);
	RootComponent = ArrowComponent;
}

void ATutorialVector::BeginPlay() {
	Super::BeginPlay();

	ArrowComponent->SetRelativeScale3D(InitialScale);
}

void ATutorialVector::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (Delay > 0.0f) {
		Delay -= DeltaTime;
		return;
	}

	if (ElapsedTime < Duration) {
		ElapsedTime += DeltaTime;
		const float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);
		const FVector NewScale = FMath::Lerp(InitialScale, TargetScale, Alpha);
		ArrowComponent->SetRelativeScale3D(NewScale); // Set scale.
		ArrowComponent->SetArrowColor(FLinearColor::LerpUsingHSV(StartColor, EndColor, Alpha)); // Set color.
		ArrowComponent->SetArrowLength((EndLocation - ArrowComponent->GetComponentLocation()).Size()); // Set length.
	} else {
		SetActorTickEnabled(false);
	}
}

void ATutorialVector::StartAnimation(const FVector& InOrigin, const FVector& InEnd,
                                     const FVector& InInitialScale, const FVector& InTargetScale,
                                     const FLinearColor& InStartColor, const FLinearColor& InEndColor,
                                     const float InDelay, const float InDuration) {
	EndLocation = InEnd; // Set end location.
	TargetScale = InTargetScale; // Set target scale.
	Delay = InDelay; // Set delay.
	Duration = InDuration; // Set duration.
	ElapsedTime = 0.0f; // Reset time.
	InitialScale = InInitialScale;
	ArrowComponent->SetRelativeScale3D(InInitialScale); // Reset scale.
	ArrowComponent->SetRelativeRotation(FRotationMatrix::MakeFromX(InEnd - InOrigin).Rotator()); // Set rotation.
	ArrowComponent->SetRelativeLocation(InOrigin); // Place at the expected location.
	StartColor = InStartColor; // Set start color.
	EndColor = InEndColor; // Set end color.
}
