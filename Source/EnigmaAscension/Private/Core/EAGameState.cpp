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

void AEAGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEAGameState,bGameStarted);
	DOREPLIFETIME(AEAGameState,ScoreTeamA);
	DOREPLIFETIME(AEAGameState,ScoreTeamB);
}

TArray<FPlayerStatistics> AEAGameState::GetPlayerStatistics()
{
	TArray<FPlayerStatistics> PlayerStats;
	for (auto Player : PlayerArray)
	{
		PlayerStats.Add(Cast<AEAPlayerState>(Player)->Stats);
	}

	return PlayerStats;
}
