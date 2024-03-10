// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EAPlayerState.generated.h"

/**
 *
 * 
 */

USTRUCT(BlueprintType)
struct FPlayerStatistics
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite)
	int MatchScore = 0;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsTeamA;
};

UCLASS()
class ENIGMAASCENSION_API AEAPlayerState : public APlayerState
{
	GENERATED_BODY()
public:

	AEAPlayerState();
	UPROPERTY(Replicated)
	int MyPlayerIndex;
	UPROPERTY(Replicated,BlueprintReadWrite)// TODO: Set this in Game Mode
	bool bIsTeamA;
	UPROPERTY(Replicated,BlueprintReadWrite)
	FPlayerStatistics Stats;
	
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	int GetPlayerIndex() const;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
