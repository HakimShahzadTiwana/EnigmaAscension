// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EAPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ENIGMAASCENSION_API AEAPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int MyPlayerIndex; // TODO: Set this in Game Mode
	UFUNCTION(BlueprintCallable)
	int GetPlayerIndex() const;
};
