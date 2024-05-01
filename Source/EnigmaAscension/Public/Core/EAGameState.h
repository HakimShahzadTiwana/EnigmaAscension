// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Player/EAPlayerState.h"
#include "EAGameState.generated.h"

/**
 * 
 */

UCLASS()
class ENIGMAASCENSION_API AEAGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,ReplicatedUsing = OnRep_GameStarted)
	bool bGameStarted=false;
	UPROPERTY(BlueprintReadWrite,ReplicatedUsing = OnRep_ScoreTeamA)
	int ScoreTeamA=0;
	UPROPERTY(BlueprintReadWrite,ReplicatedUsing = OnRep_ScoreTeamB)
	int ScoreTeamB=0;
	UPROPERTY(BlueprintReadOnly)
	int ScoreLimit = 3; // TODO: Make this Const
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Timer)
	int Current_Timer = 120; // in seconds
	FTimerHandle TimerHandle_DecrementTimer;
	
	UFUNCTION()
	virtual void OnRep_GameStarted();
	UFUNCTION()
	virtual void OnRep_ScoreTeamA();
	UFUNCTION()
	virtual void OnRep_ScoreTeamB();
	UFUNCTION()
	virtual void OnRep_Timer();
	UFUNCTION(Server,Reliable)
	void Server_IncrementTeamScore(bool bIsTeamA);
	UFUNCTION()
	virtual void BeginPlay() override;
	UFUNCTION(Server,Reliable)
	void Server_BeginTimer();
	UFUNCTION(NetMulticast,Reliable)
	void DecrementTimer();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FPlayerStatistics> GetPlayerStatistics();
	
};


