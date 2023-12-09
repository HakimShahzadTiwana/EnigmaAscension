// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
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

	UFUNCTION()
	virtual void OnRep_GameStarted();
	UFUNCTION()
	virtual void OnRep_ScoreTeamA();
	UFUNCTION()
	virtual void OnRep_ScoreTeamB();

	void IncrementTeamScore(bool bIsTeamA);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
};


