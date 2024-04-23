// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Networking/EASessionInterface.h"
#include "EAGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFriendsRetrieved);


USTRUCT(BlueprintType)
struct FSessionData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	FString Name;
	UPROPERTY(BlueprintReadWrite)
	int CurrentPlayers;
	UPROPERTY(BlueprintReadWrite)
	int MaxPlayers;
	UPROPERTY(BlueprintReadWrite)
	FString CreationTime;
	UPROPERTY(BlueprintReadWrite)
	FString SessionPassword;
	UPROPERTY(BlueprintReadWrite)
	bool isPublic;
};

/**
 * Used for creating sessions (Implementation in blueprints using Advanced Session Plugin)
 */
DECLARE_LOG_CATEGORY_EXTERN(LogEANetworking,Log,All);
UCLASS()
class ENIGMAASCENSION_API UEAGameInstance : public UGameInstance, public IEASessionInterface
{
	GENERATED_BODY()
public:
	UEAGameInstance();
	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;
	const FName EA_SESSION_NAME = "EA_Game_Session";
	const FName EA_SESSION_MAIN_FILTER_KEY = "EA_Session";
	const FString EA_SESSION_MAIN_FILTER_VALUE = "EA_Simple_Session";

	UPROPERTY(BlueprintReadWrite)
	bool bIsRollbackSet =false;

	// Session Data to send forward. Can be sorted/filtered/searched
	UPROPERTY(BlueprintReadWrite)
	TArray<FSessionData> SessionData;

	// Session Data retrieved from Steam Sessions
	UPROPERTY(BlueprintReadOnly)
	TArray<FSessionData> FoundSessions;

	
protected:
	
	virtual void Init() override;
	
	//Using Advanced Session Interfaces in Child BP
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void CreateSession(bool isPublic,const FString &Password);
	
	//Using Advanced Session Interfaces in Child BP
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void FindSessions();

	UFUNCTION(Exec)
	void SetRollBack(bool bIsRollback);

	UFUNCTION(BlueprintCallable)
	TArray<FSessionData> SearchSessionData(FString UserQuery);
	TArray<FSessionData> SearchSessions(FString UserQuery,TArray<FSessionData> SessionsToSearch);
	
	UFUNCTION(BlueprintCallable)
	TArray<FSessionData> FilterSessionData(bool publicMatch, FString Hostname);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	int GetAge(FString CreationTime);

	UFUNCTION(BlueprintCallable)
	TArray<FSessionData> SortSessionData();

	void PrintSessionData (TArray<FSessionData> data);
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
	FString GetDateTime();

	UFUNCTION(BlueprintCallable)
	void SaveSteamSessionsFound(TArray<FSessionData> data);
	

	FOnReadFriendsListComplete OnReadFriends;
	UPROPERTY(BlueprintAssignable)
	FOnFriendsRetrieved OnFriendsRetrieved;
	UFUNCTION(BlueprintCallable)
	void ReadFriendListData();
    void OnAllFriendsRead(int I, bool bArg, const FString& String, const FString& String1);
	
	// Display all matches that are made by friends.
	TArray< TSharedRef<FOnlineFriend> > FriendList;


	// Testing 
	UFUNCTION(Exec)
	void TestSorting();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void LoadSortingTest();
	

	
};
