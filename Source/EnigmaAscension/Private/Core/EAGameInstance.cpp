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

TArray<FSessionData> UEAGameInstance::SearchSessions(FString UserQuery,TArray<FSessionData> SessionsToSearch)
{
	TArray<FSessionData> SearchedSessionData;
	if (SessionsToSearch.IsEmpty())
	{
		return SearchedSessionData;
	}
	
	for (auto data : SessionsToSearch)
	{
		if(data.Name.Contains(UserQuery))
		{
			SearchedSessionData.Add(data);
			UE_LOG(LogEANetworking, Log, TEXT("%hs - Found %s from search query : %s"), __FUNCTION__,*data.Name,*UserQuery);

		}
	}
	return SearchedSessionData;
}

void UEAGameInstance::ReadFriendListData()
{
	//Read friends first
	IOnlineFriendsPtr FriendsInterface = IOnlineSubsystem::Get()->GetFriendsInterface();
	if(FriendsInterface == nullptr)
	{
		UE_LOG(LogCore, Error, TEXT("%hs - Couldnt find Friends Interface"), __FUNCTION__);
		return;
	}
	if(FriendsInterface->ReadFriendsList(0,EFriendsLists::ToString((EFriendsLists::Default)),OnReadFriends.CreateUObject(this,&UEAGameInstance::OnAllFriendsRead)))
	{
		UE_LOG(LogCore, Log, TEXT("%hs - Reading Friends List..."), __FUNCTION__);
	}else
	{
		UE_LOG(LogCore, Warning, TEXT("%hs - Couldnt read friend list..."), __FUNCTION__);
	}
}


TArray<FSessionData> UEAGameInstance::SearchSessionData(FString UserQuery)
{
	return SearchSessions(UserQuery,FoundSessions);
}

void UEAGameInstance::OnAllFriendsRead(int I, bool bArg, const FString& String, const FString& String1)
{
	IOnlineFriendsPtr FriendsInterface = IOnlineSubsystem::Get()->GetFriendsInterface();
	FriendsInterface->GetFriendsList(0,EFriendsLists::ToString((EFriendsLists::Default)),FriendList);
	UE_LOG(LogCore, Log, TEXT("%hs - Stored Friends List"), __FUNCTION__);
	for (auto F : FriendList)
	{
		UE_LOG(LogCore, Log, TEXT("%hs - %s"), __FUNCTION__,*F->GetRealName());
	}
	OnFriendsRetrieved.Broadcast();

}

TArray<FSessionData> UEAGameInstance::FilterSessionData(bool publicMatch, FString Hostname)
{
	TArray<FSessionData> FilteredMatch;

	// Get matches that are public/private first
	for (auto Match : FoundSessions)
	{
		if(Match.isPublic == publicMatch)
		{
			FilteredMatch.Add(Match);
		}
	}

	// Out of the filtered Matches, get the ones that are similar to the host name
	if(!Hostname.IsEmpty())
	{
		FilteredMatch = SearchSessions(Hostname,FilteredMatch);
	}

	if(!publicMatch && !FriendList.IsEmpty())
	{
		TArray<FSessionData> FinalFilter;
		for (auto Friend : FriendList)
		{
			TArray<FSessionData> FriendSessions = SearchSessions(Friend->GetRealName(),FilteredMatch);
			if(FriendSessions.Num() > 0)
			{
				FinalFilter.Append(FriendSessions);
			}
		}
		FilteredMatch = FinalFilter;
	}

	return FilteredMatch;
		


	
	
}

int UEAGameInstance::GetAge(FString CreationTime)
{
	FDateTime Date;
	FDateTime CurrentTime = FDateTime::Now();
	FDateTime::Parse(CreationTime,Date);
	FTimespan SessionAge = CurrentTime - Date;

	return SessionAge.GetMinutes();
	
}

TArray<FSessionData> UEAGameInstance::SortSessionData()
{
	TArray<FSessionData> SortedMatch;
	if (FoundSessions.IsEmpty())
	{
		return SortedMatch;
	}
	
	SortedMatch.Add(FoundSessions[0]);
	FDateTime CurrentTime = FDateTime::Now();
	for (int i = 1 ; i<FoundSessions.Num();i++)
	{
		FDateTime SessionDate;
		FDateTime::Parse(FoundSessions[i].CreationTime,SessionDate);
		FTimespan SessionAge = CurrentTime - SessionDate;
		for(int j = 0 ; j < SortedMatch.Num(); j++)
		{
			FDateTime SortDate;
			FDateTime::Parse(SortedMatch[i].CreationTime,SortDate);
			FTimespan SortAge = CurrentTime - CurrentTime;

			if(SessionAge < SortAge)
			{
				SortedMatch.Insert(FoundSessions[i],j);
				break;
			}
		}
	}

	UE_LOG(LogEANetworking, Log, TEXT("%hs -------------------------Session Data --------------------"), __FUNCTION__);
	PrintSessionData(FoundSessions);
	UE_LOG(LogEANetworking, Log, TEXT("%hs -------------------------Sorted Data --------------------"), __FUNCTION__);
	PrintSessionData(SortedMatch);
	
	return SortedMatch;
}

FString UEAGameInstance::GetDateTime()
{
	FDateTime DateTime;
	return DateTime.Now().ToString();
}

void UEAGameInstance::SaveSteamSessionsFound(TArray<FSessionData> data)
{
	FoundSessions = data;
}

void UEAGameInstance::PrintSessionData (TArray<FSessionData> data)
{
	for (auto d : data)
	{
		UE_LOG(LogEANetworking, Log, TEXT("%hs - Name : %s, Creation : %s, Public : %d"), __FUNCTION__,*d.Name,*d.CreationTime,true);
	}
	
}


