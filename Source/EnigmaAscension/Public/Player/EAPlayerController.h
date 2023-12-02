// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnigmaAscension/EnigmaAscension.h"
#include "GameFramework/PlayerController.h"
#include "EAPlayerController.generated.h"

/**
 * 
 */
USTRUCT()
struct FInputID
{
	GENERATED_BODY()
	int Frame;
	// Owning Player Index
	int InstigatorControllerID;
};

USTRUCT()
struct FPlayerInputData
{
	GENERATED_BODY()
	FInputID PlayerInputID;
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
	UPROPERTY()
	int MyPlayerIndex; // TODO: Set this in Game Mode
	UPROPERTY()
	int StartFrame;
	
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

	// Roll Back Net Code
	UFUNCTION(Server, Reliable)
	PlayerInputData Client_CollectInputData();
	

	
};
