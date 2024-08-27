// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialVectorFsm.h"

void ATutorialVectorFsm::Initialize() {
	Super::Initialize();

	for (FTutorialVectorStateDataArray& Data : VectorData) {
		if (UTutorialVectorState* State = NewObject<UTutorialVectorState>(this, UTutorialVectorState::StaticClass()); IsValid(State)) {
			State->VectorData = Data.Array;
			TutorialStates.Add(State);
		}
	}
}
