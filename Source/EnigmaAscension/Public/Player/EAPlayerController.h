// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ENIGMAASCENSION_API AEAPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEAPlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void BindGasInputs();
	UPROPERTY()
	APawn* PlayerPawn;
	
	UFUNCTION()
	void MoveForward(float X);
	UFUNCTION()
	void MoveRight(float X);
	UFUNCTION()
	void Turn(float X);
	UFUNCTION()
	void LookUp(float X);
	UFUNCTION()
	void StartJump();
	UFUNCTION()
	void StopJump();
	UFUNCTION()
	void StartSprint();
	UFUNCTION()
	void StopSprint();


	

	
};
