// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialGameModeBase.h"

#include "TutorialPlayerController.h"

ATutorialGameModeBase::ATutorialGameModeBase() {
	// Set the player controller class.
	PlayerControllerClass = ATutorialPlayerController::StaticClass();
}
