// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialPlayerController.h"

#include "EngineUtils.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "TutorialFsm.h"
#include "TutorialState.h"

void ATutorialPlayerController::BeginPlay() {
	Super::BeginPlay();

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()); IsValid(Subsystem)) {
		Subsystem->AddMappingContext(TutorialInputContext, 0);
	}

	// Find all FSMs in the world.
	for (TActorIterator<ATutorialFsm> It(GetWorld()); It; ++It) {
		(*It)->Initialize();
		TutorialFSMs.Add(*It);
	}
}

void ATutorialPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent); IsValid(EnhancedInputComponent)) {
		EnhancedInputComponent->BindAction(NextTutorialAction, ETriggerEvent::Triggered, this, &ATutorialPlayerController::OnNextTutorialInputAction);
	}
}

void ATutorialPlayerController::OnNextTutorialInputAction() {
	if (CurrentTutorialFsmIndex < TutorialFSMs.Num() && !TutorialFSMs.IsEmpty()) {
		// Check if the current FSM is valid.
		if (CurrentTutorialFsmIndex >= 0 && CurrentTutorialFsmIndex < TutorialFSMs.Num()) {
			// Check if current FSM has no more states.
			if (TutorialFSMs[CurrentTutorialFsmIndex]->IsFinished()) {
				// Stop the current FSM.
				TutorialFSMs[CurrentTutorialFsmIndex]->Stop();
			}

			TutorialFSMs[CurrentTutorialFsmIndex]->Stop();
		}

		// Start the next FSM.
		++CurrentTutorialFsmIndex;
		if (CurrentTutorialFsmIndex >= TutorialFSMs.Num()) {
			CurrentTutorialFsmIndex = 0;
		}

		TutorialFSMs[CurrentTutorialFsmIndex]->Execute();
	}
}
