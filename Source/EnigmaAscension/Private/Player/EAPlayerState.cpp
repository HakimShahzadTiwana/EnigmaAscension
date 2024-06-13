// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EAPlayerState.h"

#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/EAPlayerController.h"


AEAPlayerState::AEAPlayerState()
{
	SetShouldUpdateReplicatedPing(true);
	SetReplicates(true);
}

void AEAPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

int AEAPlayerState::GetPlayerIndex() const
{
	
	return MyPlayerIndex;
}

void AEAPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEAPlayerState, MyPlayerIndex);
	DOREPLIFETIME(AEAPlayerState, bIsTeamA);
	DOREPLIFETIME(AEAPlayerState, Stats);
}
