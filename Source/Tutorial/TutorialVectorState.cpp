// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialVectorState.h"

#include "Engine/World.h"

void UTutorialVectorState::Execute(AActor* WorldContext) {
	Super::Execute(WorldContext);

	if (!IsValid(WorldContext)) {
		UE_LOG(LogTemp, Error, TEXT("UTutorialVectorState::Execute: WorldContext is not valid."));
		return;
	}

	auto* const World = WorldContext->GetWorld();
	if (!IsValid(World)) {
		UE_LOG(LogTemp, Error, TEXT("UTutorialVectorState::Execute: World is not valid."));
		return;
	}

	auto* PC = World->GetFirstPlayerController();
	if (!IsValid(PC)) {
		UE_LOG(LogTemp, Error, TEXT("UTutorialVectorState::Execute: PlayerController is not valid."));
		return;
	}

	PC->SetControlRotation(PlayerRotation);

	auto* Pawn = PC->GetPawn();
	if (!IsValid(Pawn)) {
		UE_LOG(LogTemp, Error, TEXT("UTutorialVectorState::Execute: Pawn is not valid."));
		return;
	}

	Pawn->SetActorLocation(PlayerLocation);

	for (const auto& Data : VectorData) {
		if (auto* VectorActor = World->SpawnActor<ATutorialVector>(VectorActorClass, Data.StartLocation, FRotator::ZeroRotator)) {
			VectorActor->StartAnimation(Data.StartLocation, Data.FinishLocation,
			                            Data.InitialScale, Data.TargetScale,
			                            Data.StartColor, Data.EndColor,
			                            Data.Delay, Data.Duration, Data.Scaling);
			Actors.Add(VectorActor);
		}
	}
}

void UTutorialVectorState::Stop() {
	Super::Stop();

	for (auto* Actor : Actors) {
		if (IsValid(Actor)) {
			Actor->Destroy();
		}
	}
}
