// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialVectorState.h"

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

	for (const auto& Data : VectorData) {
		if (auto* VectorActor = World->SpawnActor<ATutorialVector>(VectorActorClass, Data.StartLocation, FRotator::ZeroRotator)) {
			VectorActor->StartAnimation(Data.StartLocation, Data.FinishLocation,
			                            Data.InitialScale, Data.TargetScale,
			                            Data.StartColor, Data.EndColor,
			                            Data.Delay, Data.Duration);
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
