// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Networking/EASessionInterface.h"
#include "EAGameInstance.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(LogEANetworking,Log,All);
UCLASS()
class ENIGMAASCENSION_API UEAGameInstance : public UGameInstance, public IEASessionInterface
{
	GENERATED_BODY()
public:
	UEAGameInstance();
	
	const FName EA_SESSION_NAME = "EA_Game_Session";
	const FName EA_SESSION_MAIN_FILTER_KEY = "EA_Session";
	const FString EA_SESSION_MAIN_FILTER_VALUE = "EA_Simple_Session";
protected:
	
	virtual void Init() override;
	
	//Using Advanced Session Interfaces in Child BP
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void CreateSession();
	
	//Using Advanced Session Interfaces in Child BP
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void FindSessions();

	//Using Advanced Session Interfaces in Child BP
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void JoinSession();
	
	
	
	
	
	
	
	
	

	
};
