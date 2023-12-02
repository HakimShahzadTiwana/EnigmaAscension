// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnigmaAscension/EnigmaAscension.h"
#include "GameFramework/PlayerController.h"
#include "EAPlayerController.generated.h"

/**
 * 
 */
struct InputID
{
	int Frame;
	// Owning Player Index
	int InstigatorControllerID;
};

struct PlayerInputData
{
	InputID PlayerInputID;
	EEAAbilityInput InputType;
	// Target Player Index
	int TargetControllerID;
	float Timestamp;
	float ClientPing;
};

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
