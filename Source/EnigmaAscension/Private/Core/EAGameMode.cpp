// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/EAGameMode.h"

#include <Player/EAPlayerState.h>

#include "Core/EAGameInstance.h"
#include "DSP/AudioDebuggingUtilities.h"
#include "Kismet/GameplayStatics.h"
#include "Player/EACharacter.h"

void AEAGameMode::OnPostLogin(AController* NewPlayer)
{
	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::OnPostLogin"));
	Super::OnPostLogin(NewPlayer);
	
	AEAPlayerController* EA_Controller  = Cast<AEAPlayerController>(NewPlayer);
	if(!IsValid(EA_Controller)){
		UE_LOG(LogGameMode, Error, TEXT("AEAGameMode::OnPostLogin Cast To AEAPlayerController Failed"));
	}

	bool AssignedTeam =  ((GetNumPlayers()-1) % 2 == 0) ? true : false;
	
	ConnectedPlayerNames.Add(FString::Printf(TEXT("Player %d"),GetNumPlayers()));
	ConnectedPlayerTeams.Add(AssignedTeam);
	
	// Set Player Team
	EA_Controller->bIsTeamA = AssignedTeam;
	EA_Controller->GetPlayerState<AEAPlayerState>()->bIsTeamA = AssignedTeam;
	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::OnPostLogin Player Index %d is Team %d"),(GetNumPlayers()-1),EA_Controller->bIsTeamA);
	// Create Client UI - RPC Call
	EA_Controller->Client_CreateHUD();
	
	// Update Lobby UI
	UEAGameInstance* GameInstance = Cast<UEAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	for (int i = 0; i < GetNumPlayers(); i++){
		Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),i))->
		Client_UpdateTeamUI(ConnectedPlayerNames,ConnectedPlayerTeams);
	}
	// // Tell the player state the index of its controller
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
	// OnTargetFound.AddDynamic(this,&AEAGameMode::Rollback);
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
	// UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::AddInputToBuffer"));
	// UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::AddInputToBuffer - Server Current Time is : %d"),ServerTimeInMs);
	// UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::AddInputToBuffer - Client Ping is : %d"), Data.ClientPing);
	int RewindIndex = InputBuffer.Num()-1-(Data.ClientPing/2);
//	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::AddInputToBuffer -Inserting Sent data to Index : %d"),RewindIndex);
	if (RewindIndex>0 && RewindIndex<InputBuffer.Num())
	{
		InputBuffer[RewindIndex] = Data;
	}
	else
	{
	//	UE_LOG(LogGameMode, Warning, TEXT("%hs - Rewind Index out of bounds : %d"), __FUNCTION__,RewindIndex);
		return;

	}

	if(Data.ClientPing>RollbackPingThreshold)
	{
		// For inputs in the (Rewind time to Current time) window
		for(int i = RewindIndex ; i<InputBuffer.Num();i++)
		{
			// If the existing inputs were targeted at this instigator then count that as relevant information for rollback
			if(InputBuffer[i].TargetControllerID == Data.PlayerInputID.InstigatorControllerID)
			{
				//UE_LOG(LogGameMode, Log, TEXT("Found Relevant Information at index %d"),i);
				// Store the relevant information in a TMap that contains arrays of relevant inputs for each player controller
				RelevantInputs.FindOrAdd(Data.PlayerInputID.InstigatorControllerID).Add(InputBuffer[i]);
				
				// Once the relevant controllers are found we have to wait for the server to check what the target
				// of the attack was. So lets just store relevant information in an array then wait for the target.
				// Once the target is found from the logic in the character class, we can then call a delegate to check
				// if the instigator id's of the relevant information match the id's of the current attacker.
				// if they do, we have found where we exactly have to rollback. We will revert any damage that the
				// instigator did to the current player.
			}
		}
		
	}
	
	
}

void AEAGameMode::UpdateInputBuffer()
{
	ServerTimeInMs++;
	FPlayerInputData Data;

	// Check to see if we want to print buffer on update
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
	// if(InputBuffer.Num()>InputBufferLogSize)
	// {
	// 	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead"));
	// 	UE_LOG(LogGameMode, Log, TEXT("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"));
	// 	for (int i=0 ; i< InputBufferLogSize ; i++)
	// 	{
	// 		FPlayerInputData Data = InputBuffer[i];
	// 		UE_LOG(LogGameMode, Log, TEXT("--------------------------------------------------"));
	// 		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead - Player Controller ID : %d"),Data.PlayerInputID.InstigatorControllerID);
	// 		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead - Frame : %d"),Data.PlayerInputID.Frame);
	// 		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead - Player Ping : %d"), Data.ClientPing);
	// 		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead - TimeStamp : %f"), Data.Timestamp);
	// 		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead - Target Index : %d"), Data.TargetControllerID);
	// 		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferHead - Input was : %s with value %s"),*UEnum::GetDisplayValueAsText(Data.InputType).ToString(),*Data.InputValue.ToString());
	//
	// 		UE_LOG(LogGameMode, Log, TEXT("--------------------------------------------------"));
	// 	}
	// }
	
}

void AEAGameMode::PrintBufferTail()
{
	// if(InputBuffer.Num()>InputBufferLogSize)
	// {
	// 	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail"));
	// 	UE_LOG(LogGameMode, Log, TEXT("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"));
	// 	for (int i=InputBuffer.Num()-InputBufferLogSize ; i< InputBuffer.Num() ; i++)
	// 	{
	// 		FPlayerInputData Data = InputBuffer[i];
	// 		UE_LOG(LogGameMode, Log, TEXT("--------------------------------------------------"));
	// 		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail - Player Controller ID : %d"),Data.PlayerInputID.InstigatorControllerID);
	// 		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail - Frame : %d"),Data.PlayerInputID.Frame);
	// 		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail - Player Ping : %d"), Data.ClientPing);
	// 		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail - TimeStamp : %f"), Data.Timestamp);
	// 		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail - Target Index : %d"), Data.TargetControllerID);
	// 		UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferTail - Input was : %s with value %s"),*UEnum::GetDisplayValueAsText(Data.InputType).ToString(),*Data.InputValue.ToString());
	//
	// 		UE_LOG(LogGameMode, Log, TEXT("--------------------------------------------------"));
	// 		
	// 	}
	//}
}

void AEAGameMode::PrintBufferSnapShot()
{
	// for (int i=0 ; i< InputBuffer.Num() ; i++)
	// {
	// 	FPlayerInputData Data = InputBuffer[i];
	// 	UE_LOG(LogGameMode, Log, TEXT("--------------------------------------------------"));
	// 	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferSnapshot - Player Controller ID : %d"),Data.PlayerInputID.InstigatorControllerID);
	// 	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferSnapshot - Frame : %d"),Data.PlayerInputID.Frame);
	// 	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferSnapshot - Player Ping : %d"), Data.ClientPing);
	// 	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferSnapshot - TimeStamp : %f"), Data.Timestamp);
	// 	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferSnapshot - Target Index : %d"), Data.TargetControllerID);
	// 	UE_LOG(LogGameMode, Log, TEXT("AEAGameMode::PrintBufferSnapshot - Input was : %s with value %s"),*UEnum::GetDisplayValueAsText(Data.InputType).ToString(),*Data.InputValue.ToString());
	// 	UE_LOG(LogGameMode, Log, TEXT("--------------------------------------------------"));
	// 		
	// }
}

void AEAGameMode::Rollback(int InstigatorID, int TargetID)
{
	UE_LOG(LogGameMode, Log, TEXT("Target was found, going to rollback based on relevant inputs."));
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("Rolling Back...")));
	// Find the array that contains relevant information for the instigator
	// This relevant information is basically the other players that had targeted the instigator
	if(TArray<FPlayerInputData> *Temp = RelevantInputs.Find(InstigatorID))
	{
		// Iterate through the array and find out the target player had sent an input for attacking the instigator
		for (auto It = Temp->CreateIterator(); It;++It)
		{
			if(It->PlayerInputID.InstigatorControllerID == TargetID)
			{
				UE_LOG(LogGameMode, Log, TEXT("Point of rollback found at index %d, for player controller id %d"), It.GetIndex(),InstigatorID);
				// Rollback the damage done to the player if they attacked first but was late due to network issues
				Cast<AEACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),InstigatorID))->ApplyEffectToSelf(GetRollbackHealthAmount(It->InputType));
				Cast<AEACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),InstigatorID))->RollbackAnimation(It->InputType,It->ClientPing);
				UE_LOG(LogGameMode, Log, TEXT("%hs - Rollbacked the players health and animation"), __FUNCTION__);
				
			}
		}
	}
	else
	{
		UE_LOG(LogGameMode, Log, TEXT("No relevant inputs were found for instigator with id : %d"),InstigatorID);
	}
}

void AEAGameMode::OnGameWon(bool bIsTeamA)
{
	if(bIsTeamA)
	{
		
	}else
	{
		
	}
}

TSubclassOf<UGameplayEffect>  AEAGameMode::GetRollbackHealthAmount(EEAAbilityInput attackType)
{
	switch(attackType)
	{
	case EEAAbilityInput::PrimaryAttack:

		return PrimaryHealth;
		

	case EEAAbilityInput::SecondaryAttack:

		return SecondaryHealth;
		

	case EEAAbilityInput::UltimateAbility:
		
		return UltimateHealth;

	case EEAAbilityInput::SimpleAbility:

		return SimpleAbility;
		
	default:
		return 0;
	}
	
	
}