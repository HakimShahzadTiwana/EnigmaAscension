// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/EAGameMode.h"

#include <Player/EAPlayerState.h>

#include "Kismet/GameplayStatics.h"

void AEAGameMode::OnPostLogin(AController* NewPlayer)
{
	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::OnPostLogin"));
	Super::OnPostLogin(NewPlayer);
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

void AEAGameMode::AddInputToBuffer(FPlayerInputData Data)
{
	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::AddInputToBuffer"));
	
	const int* ClientStart = ClientStartFrame.Find(Data.PlayerInputID.InstigatorControllerID);
	UE_LOG(LogGameMode, Log, TEXT("Client Start is : %d"),*ClientStart);

	UE_LOG(LogGameMode, Log, TEXT("Server Frame Count was : %d"),Server_FrameCount);
	
	const int RealFrame = *ClientStart + Data.PlayerInputID.Frame;
	UE_LOG(LogGameMode, Log, TEXT("Real Frame is : %d"),RealFrame);
	
	
	if(InputBuffer.Find(RealFrame))
	{
		InputBuffer.Find(RealFrame)->Add(Data);
	}
	else
	{
		UE_LOG(LogGameMode, Warning, TEXT("Real Frame is out of Bounds %d from %d"),RealFrame,InputBuffer.Num());
	}
	
	
}
