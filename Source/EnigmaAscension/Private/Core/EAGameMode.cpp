// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/EAGameMode.h"

#include <Player/EAPlayerState.h>

#include "Kismet/GameplayStatics.h"

void AEAGameMode::OnPostLogin(AController* NewPlayer)
{
	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::OnPostLogin"));
	Super::OnPostLogin(NewPlayer);
	// UI
	Cast<AEAPlayerController>(NewPlayer)->Client_CreateHUD();

	// Tell the player state the index of its controller
	Cast<AEAPlayerState>(Cast<AEAPlayerController>(NewPlayer)->PlayerState)->MyPlayerIndex = GetNumPlayers()-1;
	UE_LOG(LogGameMode, Log, TEXT("SetPlayer Index to %d"),GetNumPlayers()-1);

	// Keep track of the frame that the server was on when the player joined the match
	UE_LOG(LogGameMode, Log, TEXT("Player with index %d join on server frame %d"),GetNumPlayers()-1,Server_FrameCount);
	ClientStartFrame.Add(GetNumPlayers()-1,Server_FrameCount);
}

void AEAGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// Keep track of the number of frames
	Server_FrameCount++;
	
	// Add slots for each frame in the input buffer
	InputBuffer.Add(Server_FrameCount);
}

void AEAGameMode::AddInputToBuffer(const FPlayerInputData& Data)
{
	// int ClientFrames, int InstigatorID, EEAAbilityInput InputType, int TargetID, float timestamp, float ping
	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::AddInputToBuffer"));
	UE_LOG(LogGameMode, Log, TEXT("Server Frame Count was : %d"),Server_FrameCount);

	// Get the server frame that the client joined on 
	const int* ClientStart = ClientStartFrame.Find(Data.PlayerInputID.InstigatorControllerID);
	UE_LOG(LogGameMode, Log, TEXT("Client Start is : %d"),*ClientStart);

	// Add the client frame with the frame on the local machine (this might be wrong will have to confirm, but the FPS for all machines are kept at 30 rn)
	const int RealFrame = *ClientStart + Data.PlayerInputID.Frame;
	UE_LOG(LogGameMode, Log, TEXT("Real Frame is : %d"),RealFrame);
	
	
	if(InputBuffer.Find(RealFrame))
	{
		/* FPlayerInputData Data;
		// Data.Timestamp = timestamp;
		// Data.ClientPing = ping;
		// Data.InputType = InputType;
		// Data.TargetControllerID = TargetID;
		// Data.PlayerInputID.Frame = ClientFrames;
		// Data.PlayerInputID.InstigatorControllerID = InstigatorID;*/
		// Add Data send to the real frame that was calculated
		InputBuffer.Find(RealFrame)->Add(Data);
	}
	else
	{
		UE_LOG(LogGameMode, Warning, TEXT("Real Frame is out of Bounds %d from %d"),RealFrame,InputBuffer.Num());
	}
	
	
}
