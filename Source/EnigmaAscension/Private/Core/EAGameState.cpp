// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/EAGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/EAPlayerController.h"

void AEAGameState::OnRep_GameStarted()
{
	UE_LOG(LogGameState, Log, TEXT("On_Rep GameStarted"));
	Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->PlayerHUD->HostStartGame();
}


void AEAGameState::Server_IncrementTeamScore_Implementation(bool bIsTeamA)
{
	  
	if(bIsTeamA)
	{
		ScoreTeamB++;
		UE_LOG(LogGameMode, Log, TEXT("%hs - Incrementing Score for Team B to %d"), __FUNCTION__,ScoreTeamB);

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
