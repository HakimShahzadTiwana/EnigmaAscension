// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EAGameInstance.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(LogEANetworking,Log,All);
UCLASS()
class ENIGMAASCENSION_API UEAGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UEAGameInstance();
protected:
	
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
	virtual void Init() override;
	
	virtual void OnCreateSessionCompleted(FName SessionName, bool Succeeded);
	virtual void OnFindSessionCompleted(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	UFUNCTION(BlueprintCallable)
	void CreateServer();
	UFUNCTION(BlueprintCallable)
	void JoinServer();
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void BP_CreateServer();
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void BP_JoinServer();
};
