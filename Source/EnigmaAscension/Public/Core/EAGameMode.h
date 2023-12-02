// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/EAPlayerController.h"
#include "EAGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ENIGMAASCENSION_API AEAGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// Initialize values that are to be saved to players here
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Tick(float DeltaSeconds) override;
	int Server_FrameCount = 0;
	// Stores inputs of clients here
	TMap<int,TArray<FPlayerInputData>> InputBuffer;
	TMap<int,int> ClientStartFrame;
	
	// Adds Inputs to the InputBuffer
	void AddInputToBuffer(int ClientFrames, int InstigatorID, EEAAbilityInput InputType, int TargetID, float timestamp, float ping);
};
