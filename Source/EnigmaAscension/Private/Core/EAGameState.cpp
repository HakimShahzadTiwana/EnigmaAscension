// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/EAGameState.h"

#include "Core/EAGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/EAPlayerController.h"
#include "Player/EAPlayerState.h"

void AEAGameState::OnRep_GameStarted()
{
	UE_LOG(LogGameState, Log, TEXT("On_Rep GameStarted"));
	Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->PlayerHUD->HostStartGame();
	Cast<AEAPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0))->Stats.MatchScore = 0;
}


void AEAGameState::Server_IncrementTeamScore_Implementation(bool bIsTeamA)
{
	
	if(bIsTeamA)
	{
		ScoreTeamB++;
		UE_LOG(LogGameMode, Log, TEXT("%hs - Incrementing Score for Team B to %d"), __FUNCTION__,ScoreTeamB);
		if(ScoreTeamB >= ScoreLimit)
		{
			// Team B Won
			AEAGameMode* EA_GameMode = Cast<AEAGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if(IsValid(EA_GameMode))
			{
				EA_GameMode->Notify_GameWon.Broadcast(bIsTeamA);
			}
			else
			{
				UE_LOG(LogGameMode, Log, TEXT("%hs - Failed to Cast GameMode - Unable to Call Game Won Delegate"), __FUNCTION__);
			}
		}
		if(GetWorld()->IsNetMode(NM_ListenServer))
		{
			UE_LOG(LogGameMode, Log, TEXT("%hs - Updating UI to increment score for Team B by calling Client RPC"), __FUNCTION__);
			//Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->PlayerHUD->UpdateTeamScore(false,ScoreTeamB);
			OnRep_ScoreTeamB();
		}
	}
	else
	{
		UE_LOG(LogGameMode, Log, TEXT("%hs - Incrementing Score for Team A to %d"), __FUNCTION__,ScoreTeamB);

		ScoreTeamA++;
		UE_LOG(LogGameMode, Log, TEXT("%hs - Updating UI to increment score for Team A by calling Client RPC"), __FUNCTION__);
		if(ScoreTeamA >= ScoreLimit)
		{
			AEAGameMode* EA_GameMode = Cast<AEAGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if(IsValid(EA_GameMode))
			{
				EA_GameMode->Notify_GameWon.Broadcast(bIsTeamA);
			}
			else
			{
				UE_LOG(LogGameMode, Log, TEXT("%hs - Failed to Cast GameMode - Unable to Call Game Won Delegate"), __FUNCTION__);
			}
			// Team A Won
		}
		if(GetWorld()->IsNetMode(NM_ListenServer))
		{
			// Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->PlayerHUD->UpdateTeamScore(true,ScoreTeamA);
			OnRep_ScoreTeamA();
		}
	}
}


void AEAGameState::OnRep_ScoreTeamA()
{
	UE_LOG(LogGameMode, Log, TEXT("%hs"), __FUNCTION__);

	Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->PlayerHUD->UpdateTeamScore(true,ScoreTeamA);
}

void AEAGameState::OnRep_ScoreTeamB()
{
	UE_LOG(LogGameMode, Log, TEXT("%hs"), __FUNCTION__);
	Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->PlayerHUD->UpdateTeamScore(false,ScoreTeamB);
}

// void AEAGameState::OnRep_Timer()
// {
// 	UE_LOG(LogGameMode, Log, TEXT("%hs"), __FUNCTION__);
// 	Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->PlayerHUD->UpdateTimer(Current_Timer);
// }

void AEAGameState::BeginPlay()
{
	Super::BeginPlay();
}

void AEAGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEAGameState,bGameStarted);
	DOREPLIFETIME(AEAGameState,ScoreTeamA);
	DOREPLIFETIME(AEAGameState,ScoreTeamB);
	DOREPLIFETIME(AEAGameState,Current_Timer);
}

TArray<FPlayerStatistics> AEAGameState::GetPlayerStatistics()
{
	TArray<FPlayerStatistics> PlayerStats;
	for (auto Player : PlayerArray)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("Player is : %s"),*Player.GetFullName()));
		UE_LOG(LogGameState, Log, TEXT("%hs - PLayer stats for %s"), __FUNCTION__,*Player->GetPlayerName());
		PlayerStats.Add(Cast<AEAPlayerState>(Player)->Stats);
	}

	return PlayerStats;
}

void AEAGameState::Server_BeginTimer_Implementation()
{
	// Start a timer to decrement Current_Timer every second
	UE_LOG(LogGameState, Log, TEXT("%hs"), __FUNCTION__);
	GetWorldTimerManager().SetTimer(TimerHandle_DecrementTimer, this, &AEAGameState::DecrementTimer_Implementation, 1.0f, true);
}

void AEAGameState::DecrementTimer_Implementation()
{
	LogClientOrServer();
	Current_Timer--;
	UE_LOG(LogGameState, Log, TEXT("%hs - Current Time: %d"), __FUNCTION__,Current_Timer);

	UE_LOG(LogGameState, Log, TEXT("%hs - Caller: %d"), __FUNCTION__);
	//TODO: Call this from GameMode 
	for (int i = 0; i < UGameplayStatics::GetNumPlayerControllers(GetWorld()); i++){
		UE_LOG(LogGameState, Log, TEXT("%hs - Num Player Controllers %d"), __FUNCTION__,UGameplayStatics::GetNumPlayerControllers(GetWorld()));
		Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),i))->Client_TimerUI(Current_Timer);
	}
	
	if (Current_Timer <= 0)
	{
		// Stop the timer if Current_Timer has reached 0
		GetWorldTimerManager().ClearTimer(TimerHandle_DecrementTimer);

		AEAGameMode *_gameMode = Cast<AEAGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if(IsValid(_gameMode)){
			_gameMode->Notify_GameWon.Broadcast(ScoreTeamA > ScoreTeamB);
		}
	}
}

void  AEAGameState::LogClientOrServer()
{
	// Check if this instance has authority (is the server or standalone game)
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("This is the Server"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("This is the Client"));
	}

	// Check the network mode explicitly
	ENetMode NetMode = GetNetMode();
	switch (NetMode)
	{
	case NM_Standalone:
		UE_LOG(LogTemp, Warning, TEXT("Running in Standalone mode"));
		break;
	case NM_Client:
		UE_LOG(LogTemp, Warning, TEXT("Running on a Client"));
		break;
	case NM_ListenServer:
		UE_LOG(LogTemp, Warning, TEXT("Running as a Listen Server"));
		break;
	case NM_DedicatedServer:
		UE_LOG(LogTemp, Warning, TEXT("Running as a Dedicated Server"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown network mode"));
		break;
	}
}