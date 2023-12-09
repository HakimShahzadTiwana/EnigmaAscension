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
		if(GIsServer)
		{
			Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->PlayerHUD->UpdateTeamScore(false,ScoreTeamB);
			//OnRep_ScoreTeamB();
		}
	}
	else
	{
		ScoreTeamA++;
		Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->PlayerHUD->UpdateTeamScore(true,ScoreTeamA);
		//OnRep_ScoreTeamA();
	}
}


void AEAGameState::OnRep_ScoreTeamA()
{
	Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->PlayerHUD->UpdateTeamScore(true,ScoreTeamA);
}

void AEAGameState::OnRep_ScoreTeamB()
{
	Cast<AEAPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->PlayerHUD->UpdateTeamScore(false,ScoreTeamB);
}

void AEAGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEAGameState,bGameStarted);
	DOREPLIFETIME(AEAGameState,ScoreTeamA);
	DOREPLIFETIME(AEAGameState,ScoreTeamB);
}
