// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TutorialPlayerController.generated.h"

class ATutorialFsm;
class UInputMappingContext;
class UInputAction;
class UTutorialState;

/**
 *
 */
UCLASS(Blueprintable)
class ATutorialPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	TArray<ATutorialFsm*> TutorialFSMs;

	UPROPERTY(VisibleAnywhere, Category = "Tutorial")
	int32 CurrentTutorialFsmIndex = -1;

	UPROPERTY(EditDefaultsOnly, Category = "Tutorial")
	UInputMappingContext* TutorialInputContext = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Tutorial")
	UInputAction* NextTutorialAction = nullptr;

	void OnNextTutorialInputAction();
};
