// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialVector.h"

#include "EngineUtils.h"
#include "TutorialTextMetadata.h"
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

	for (TActorIterator<ATutorialSizeText> It(GetWorld()); It; ++It) {
		if (IsValid(*It)) {
			SizeText = *It;
			break;
		}
	}
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
		UpdateSizeText();
	} else {
		SetActorTickEnabled(false);
	}
}

void ATutorialVector::StartAnimation(const FVector& InOrigin, const FVector& InEnd,
                                     const FVector& InInitialScale, const FVector& InTargetScale,
                                     const FLinearColor& InStartColor, const FLinearColor& InEndColor,
                                     const float InDelay, const float InDuration, const float InScaling) {
	EndLocation = InEnd; // Set end location.
	TargetScale = InTargetScale; // Set target scale.
	Delay = InDelay; // Set delay.
	Duration = InDuration; // Set duration.
	Scaling = InScaling; // Set scaling.
	ElapsedTime = 0.0f; // Reset time.
	InitialScale = InInitialScale;
	ArrowComponent->SetRelativeScale3D(InInitialScale); // Reset scale.
	ArrowComponent->SetRelativeRotation(FRotationMatrix::MakeFromX(InEnd - InOrigin).Rotator()); // Set rotation.
	ArrowComponent->SetRelativeLocation(InOrigin); // Place at the expected location.
	StartColor = InStartColor; // Set start color.
	EndColor = InEndColor; // Set end color.
}

TSharedPtr<FTutorialTextMetadata> ATutorialVector::GetSizeTextMetadata() {
	auto SizeTextMetadata = MakeShared<FTutorialTextMetadata>();
	SizeTextMetadata->SetActor(this);
	const auto ArrowEnd = ArrowComponent->GetComponentLocation() + ArrowComponent->GetForwardVector() * ArrowComponent->ArrowLength;
	const auto ArrowMid = (ArrowComponent->GetComponentLocation() + ArrowEnd) / 2.0f;
	SizeTextMetadata->SetLocation(ArrowMid);
	SizeTextMetadata->SetRotation((ArrowEnd - ArrowComponent->GetComponentLocation()).Rotation());
	SizeTextMetadata->SetText(FString::Printf(TEXT("%.2f"), ArrowComponent->ArrowLength * Scaling));
	return SizeTextMetadata;
}

void ATutorialVector::AssignSizeText(const TSharedPtr<FTutorialTextMetadata>& InSizeTextMetadata) {
	if (!InSizeTextMetadata) {
		UE_LOG(LogTemp, Error, TEXT("ATutorialVector::AssignSizeText: InSizeTextMetadata is not valid."));
		return;
	}

	if (!IsValid(SizeText)) {
		UE_LOG(LogTemp, Error, TEXT("ATutorialVector::AssignSizeText: SizeText is not valid."));
		return;
	}

	SizeText->AssignText(this, InSizeTextMetadata);
}

void ATutorialVector::UpdateSizeText() {
	ReleaseSizeText();

	TSharedPtr<FTutorialTextMetadata> SizeTextMetadata = GetSizeTextMetadata();
	if (!SizeTextMetadata) {
		UE_LOG(LogTemp, Error, TEXT("ATutorialVector::UpdateSizeText: GetSizeTextMetadata failed."));
		return;
	}

	AssignSizeText(SizeTextMetadata);
}

void ATutorialVector::ReleaseSizeText() {
	if (!IsValid(SizeText)) {
		UE_LOG(LogTemp, Error, TEXT("ATutorialVector::ReleaseSizeText: SizeText is not valid."));
		return;
	}

	SizeText->ReleaseText(this);
}
