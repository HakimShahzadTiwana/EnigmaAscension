// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/EAGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/GameUserSettings.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"
DEFINE_LOG_CATEGORY(LogEANetworking);
UEAGameInstance::UEAGameInstance()
{
	
}

void UEAGameInstance::Init()
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	UserSettings->RunHardwareBenchmark();
	UserSettings->ApplyHardwareBenchmarkResults();
}

void UEAGameInstance::SetRollBack(bool bIsRollback)
{
	bIsRollbackSet=bIsRollback;
}

TArray<FSessionData> UEAGameInstance::SearchSessionData(FString UserQuery)
{
	TArray<FSessionData> SearchedSessionData;
	for (auto data : SessionData)
	{
		if(data.Name.Contains(UserQuery))
		{
			SearchedSessionData.Add(data);
			UE_LOG(LogEANetworking, Log, TEXT("%hs - Found %s from search query : %s"), __FUNCTION__,*data.Name,*UserQuery);

		}
	}
	return SearchedSessionData;
}

TArray<FSessionData> UEAGameInstance::FilterSessionData(bool publicMatch, FString Hostname)
{
	TArray<FSessionData> FilteredMatch;
	// TODO: Ask user if they want to create a public or private match and set that in Session Data with extra setting
	// TODO: Get the list of friends from the logged in user if private was selected and search hostnames for friends and substring
	return FilteredMatch;
}

TArray<FSessionData> UEAGameInstance::SortSessionData()
{
	TArray<FSessionData> SortedMatch;
	SortedMatch.Add(SessionData[0]);
	FDateTime CurrentTime = FDateTime::Now();
	for (int i = 1 ; i<SessionData.Num();i++)
	{
		FDateTime SessionDate;
		FDateTime::Parse(SessionData[i].CreationTime,SessionDate);
		FTimespan SessionAge = CurrentTime - SessionDate;
		for(int j = 0 ; j < SortedMatch.Num(); j++)
		{
			FDateTime SortDate;
			FDateTime::Parse(SortedMatch[i].CreationTime,SortDate);
			FTimespan SortAge = CurrentTime - CurrentTime;

			if(SessionAge < SortAge)
			{
				SortedMatch.Insert(SessionData[i],j);
				break;
			}
		}
	}

	UE_LOG(LogEANetworking, Log, TEXT("%hs -------------------------Session Data --------------------"), __FUNCTION__);
	PrintSessionData(SessionData);
	UE_LOG(LogEANetworking, Log, TEXT("%hs -------------------------Sorted Data --------------------"), __FUNCTION__);
	PrintSessionData(SortedMatch);
	
	return SortedMatch;
}

FString UEAGameInstance::GetDateTime()
{
	FDateTime DateTime;
	return DateTime.Now().ToString();
}

void UEAGameInstance::PrintSessionData (TArray<FSessionData> data)
{
	for (auto d : data)
	{
		UE_LOG(LogEANetworking, Log, TEXT("%hs - Name : %s, Creation : %s, Public : %d"), __FUNCTION__,*d.Name,*d.CreationTime,true);
	}
	
}


