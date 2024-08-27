// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialFsm.h"

#include "TutorialState.h"

ATutorialFsm::ATutorialFsm() {
	PrimaryActorTick.bCanEverTick = false;
}

void ATutorialFsm::BeginPlay() {
	Super::BeginPlay();
}

bool ATutorialFsm::IsFinished() const {
	return CurrentTutorialStateIndex >= TutorialStates.Num();
}

void ATutorialFsm::Initialize() {
}

void ATutorialFsm::Execute() {
	if (CurrentTutorialStateIndex < TutorialStates.Num() && !TutorialStates.IsEmpty()) {
		if (CurrentTutorialStateIndex >= 0) {
			TutorialStates[CurrentTutorialStateIndex]->Stop();
		}

		++CurrentTutorialStateIndex;
		if (CurrentTutorialStateIndex >= TutorialStates.Num()) {
			CurrentTutorialStateIndex = 0;
		}

		TutorialStates[CurrentTutorialStateIndex]->Execute(this);
	}
}

void ATutorialFsm::Stop() {
	if (CurrentTutorialStateIndex >= 0 && CurrentTutorialStateIndex < TutorialStates.Num()) {
		TutorialStates[CurrentTutorialStateIndex]->Stop();
	}
}
