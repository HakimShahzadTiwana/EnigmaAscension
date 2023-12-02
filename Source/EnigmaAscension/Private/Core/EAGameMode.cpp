// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/EAGameMode.h"

#include <Player/EAPlayerState.h>

#include "Kismet/GameplayStatics.h"

void AEAGameMode::OnPostLogin(AController* NewPlayer)
{
	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::OnPostLogin"));
	Super::OnPostLogin(NewPlayer);
	Cast<AEAPlayerController>(NewPlayer)->Client_CreateHUD();
	Cast<AEAPlayerState>(Cast<AEAPlayerController>(NewPlayer)->PlayerState)->MyPlayerIndex = GetNumPlayers()-1;
	UE_LOG(LogGameMode, Log, TEXT("SetPlayer Index to %d"),GetNumPlayers()-1);
	
	UE_LOG(LogGameMode, Log, TEXT("Player with index %d join on server frame %d"),GetNumPlayers()-1,Server_FrameCount);
	ClientStartFrame.Add(GetNumPlayers()-1,Server_FrameCount);
}

void AEAGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Server_FrameCount++;
	// UE_LOG(LogGameMode, Log, TEXT("Server_FrameCount is : %d"),Server_FrameCount);
	InputBuffer.Add(Server_FrameCount);
}

void AEAGameMode::AddInputToBuffer(int ClientFrames, int InstigatorID, EEAAbilityInput InputType, int TargetID, float timestamp, float ping)
{
	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::AddInputToBuffer"));
	
	int* ClientStart = ClientStartFrame.Find(InstigatorID);
	UE_LOG(LogGameMode, Log, TEXT("Client Start is : %d"),*ClientStart);

	UE_LOG(LogGameMode, Log, TEXT("Server Frame Count was : %d"),Server_FrameCount);
	
	const int RealFrame = *ClientStart +ClientFrames;
	UE_LOG(LogGameMode, Log, TEXT("Real Frame is : %d"),RealFrame);
	
	
	if(InputBuffer.Find(RealFrame))
	{
		FPlayerInputData Data;
		Data.Timestamp = timestamp;
		Data.ClientPing = ping;
		Data.InputType = InputType;
		Data.TargetControllerID = TargetID;
		Data.PlayerInputID.Frame = ClientFrames;
		Data.PlayerInputID.InstigatorControllerID = InstigatorID;
		InputBuffer.Find(RealFrame)->Add(Data);
	}
	else
	{
		UE_LOG(LogGameMode, Warning, TEXT("Real Frame is out of Bounds %d from %d"),RealFrame,InputBuffer.Num());
	}
	
	
}
