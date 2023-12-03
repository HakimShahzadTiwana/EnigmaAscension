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

void AEAGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(ServerTime,this, &AEAGameMode::UpdateInputBuffer,0.1,true);
}

void AEAGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// Keep track of the number of frames
	Server_FrameCount++;
	
}

void AEAGameMode::AddInputToBuffer(const FPlayerInputData& Data)
{
	// int ClientFrames, int InstigatorID, EEAAbilityInput InputType, int TargetID, float timestamp, float ping
	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::AddInputToBuffer"));
	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::AddInputToBuffer - Server Current Time is : %d"),ServerTimeInMs);
	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::AddInputToBuffer - Client Ping is : %d"), Data.ClientPing);
	int RewindIndex = InputBuffer.Num()-1-Data.ClientPing;
	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::AddInputToBuffer -Inserting Sent data to Index : %d"),RewindIndex);
	
	InputBuffer[RewindIndex] = Data;

	if(Data.ClientPing>RollbackPingThreshold)
	{
		for(int i = RewindIndex ; i<InputBuffer.Num();i++)
		{
			if(InputBuffer[i].TargetControllerID == Data.PlayerInputID.InstigatorControllerID)
			{
				UE_LOG(LogGameMode, Log, TEXT("Found Relevant Information at index %d"),i);
			}
		}
	}
	
	
}

void AEAGameMode::UpdateInputBuffer()
{
	ServerTimeInMs++;
	FPlayerInputData Data;
	
	if(ServerTimeInMs%InputBufferLogSize==0 && bShowBufferContent)
	{
		PrintBufferHead();
		PrintBufferTail();
	}
	if(InputBuffer.Num()<BufferSize)
	{
		InputBuffer.Add(Data);
	}
	else
	{
		InputBuffer.RemoveAt(0);
		InputBuffer.Add(Data);
	}
}

void AEAGameMode::PrintBufferHead()
{
	if(InputBuffer.Num()>InputBufferLogSize)
	{
		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead"));
		UE_LOG(LogGameMode, Log, TEXT("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"));
		for (int i=0 ; i< InputBufferLogSize ; i++)
		{
			FPlayerInputData Data = InputBuffer[i];
			UE_LOG(LogGameMode, Log, TEXT("--------------------------------------------------"));
			UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead - Player Controller ID : %d"),Data.PlayerInputID.InstigatorControllerID);
			UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead - Frame : %d"),Data.PlayerInputID.Frame);
			UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead - Player Ping : %d"), Data.ClientPing);
			UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead - TimeStamp : %f"), Data.Timestamp);
			UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead - Target Index : %d"), Data.TargetControllerID);
			UE_LOG(LogGameMode, Log, TEXT("--------------------------------------------------"));
		}
	}
	
}

void AEAGameMode::PrintBufferTail()
{
	if(InputBuffer.Num()>InputBufferLogSize)
	{
		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail"));
		UE_LOG(LogGameMode, Log, TEXT("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"));
		for (int i=InputBuffer.Num()-InputBufferLogSize ; i< InputBuffer.Num() ; i++)
		{
			FPlayerInputData Data = InputBuffer[i];
			UE_LOG(LogGameMode, Log, TEXT("--------------------------------------------------"));
			UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail - Player Controller ID : %d"),Data.PlayerInputID.InstigatorControllerID);
			UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail - Frame : %d"),Data.PlayerInputID.Frame);
			UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail - Player Ping : %d"), Data.ClientPing);
			UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail - TimeStamp : %f"), Data.Timestamp);
			UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail - Target Index : %d"), Data.TargetControllerID);
			UE_LOG(LogGameMode, Log, TEXT("--------------------------------------------------"));
			
		}
	}
}

void AEAGameMode::PrintBufferSnapShot()
{
	for (int i=0 ; i< InputBuffer.Num() ; i++)
	{
		FPlayerInputData Data = InputBuffer[i];
		UE_LOG(LogGameMode, Log, TEXT("--------------------------------------------------"));
		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferSnapshot - Player Controller ID : %d"),Data.PlayerInputID.InstigatorControllerID);
		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferSnapshot - Frame : %d"),Data.PlayerInputID.Frame);
		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferSnapshot - Player Ping : %d"), Data.ClientPing);
		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferSnapshot - TimeStamp : %f"), Data.Timestamp);
		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferSnapshot - Target Index : %d"), Data.TargetControllerID);
		UE_LOG(LogGameMode, Log, TEXT("--------------------------------------------------"));
			
	}
}
