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

	// The current frame that the server is on, updated every tick
	int Server_FrameCount = 0;
	
	// Stores inputs of clients here, TArray because there is a possibility that multiple clients give inputs at the same frame
	TMap<int,TArray<FPlayerInputData>> InputBuffer;
	
	// Stores the frame the server was on when the client joined the match
	TMap<int,int> ClientStartFrame;
	
	// Adds Inputs to the InputBuffer
	void AddInputToBuffer(const FPlayerInputData& Data);
};
