// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/EAGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

DEFINE_LOG_CATEGORY(LogEANetworking);
UEAGameInstance::UEAGameInstance()
{
}

void UEAGameInstance::OnFindSessionCompleted(bool Succeeded)
{
	UE_LOG(LogEANetworking, Log, TEXT("OnFindSessionCompleted"));
	// if(Succeeded)
	// {
	// 	FString Session_Type;
	// 	
	// 	SessionSearch->QuerySettings.Get(FName("EA_Session_Type"),Session_Type);
	// 	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("%s"),*Session_Type));
	// 	TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
	// 	UE_LOG(LogEANetworking, Log, TEXT("Find Session Succeeded, found %d Sessions"),SearchResults.Num());
	// 	GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Blue, FString::Printf(TEXT("Find Session Succeeded, found %d Sessions"),SearchResults.Num()));
	// 	if(SearchResults.Num()>0)
	// 	{
	// 		UE_LOG(LogEANetworking, Log, TEXT("Joining Server..."));
	// 		SessionInterface->JoinSession(0,"My Session",SearchResults[0]);
	// 	}
	// }
	// else
	// {
	// 	UE_LOG(LogEANetworking, Warning, TEXT("Find Session Failed"));
	// }
}

void UEAGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogEANetworking, Log, TEXT("OnJoinSessionComplete"));
	if(APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0))
	{
		FString JoinAddress="";
		SessionInterface->GetResolvedConnectString(SessionName,JoinAddress);
		if(JoinAddress!="")
		{
			PlayerController->ClientTravel(JoinAddress,TRAVEL_Absolute);
		}
	}
}

void UEAGameInstance::Init()
{
	//Super::Init();
	if(IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,&UEAGameInstance::OnCreateSessionCompleted);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,&UEAGameInstance::OnFindSessionCompleted);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this,&UEAGameInstance::OnJoinSessionComplete);
		}
	}
	
}

void UEAGameInstance::OnCreateSessionCompleted(FName SessionName, bool Succeeded)
{
	UE_LOG(LogEANetworking, Log, TEXT("OnCreateSessionCompleted"));
	if(Succeeded)
	{
		UE_LOG(LogEANetworking, Log, TEXT("Create Session Succeeded"));
		FOnlineSessionSettings* Settings = SessionInterface->GetSessionSettings(SessionName);
		for (auto Setting : Settings->Settings)
		{
			UE_LOG(LogEANetworking, Log, TEXT("Name : %s, Value : %s"),*Setting.Key.ToString(), *Setting.Value.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("Name : %s, Value : %s"),*Setting.Key.ToString(), *Setting.Value.ToString()));
		}
		GetWorld()->ServerTravel("/Game/Maps/TestMap?listen");
	}
	else
	{
		UE_LOG(LogEANetworking, Warning, TEXT("Create Session Failed"));
	}
}

void UEAGameInstance::CreateServer()
{
	UE_LOG(LogEANetworking, Log, TEXT("Create Server"));
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated=false;
	SessionSettings.bIsLANMatch=false;
	SessionSettings.bShouldAdvertise=true;
	SessionSettings.bUsesPresence=true;
	SessionSettings.NumPublicConnections=8;
	FOnlineSessionSetting EA_Identifier_Setting;
	EA_Identifier_Setting.Data=FString("EA_Session");
	EA_Identifier_Setting.AdvertisementType=EOnlineDataAdvertisementType::ViaOnlineServiceAndPing;
	SessionSettings.Settings.Add(FName("EA_Session_Type"),EA_Identifier_Setting);
	SessionSettings.Set(FName("EA_Session_Type"),FString("EA_Session"),EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	SessionInterface->CreateSession(0,FName("My Session"),SessionSettings);
	
	
}

void UEAGameInstance::JoinServer()
{
	UE_LOG(LogEANetworking, Log, TEXT("Join Server"));
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults=200000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE,true,EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.SearchParams.Add(FName("EA_Session_Type"),FString("EA_Session"));
	SessionSearch->QuerySettings.Set(FName("EA_Session_Type"),FString("EA_Session"),EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0,SessionSearch.ToSharedRef());
}
