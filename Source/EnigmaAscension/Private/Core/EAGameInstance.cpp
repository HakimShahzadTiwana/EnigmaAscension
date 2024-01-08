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

void UEAGameInstance::Init()
{
	
}

void UEAGameInstance::SetRollBack(bool bIsRollback)
{
	bIsRollbackSet=bIsRollback;
}


//Using Advanced Sessions 
// void UEAGameInstance::CreateServer()
// {
// 	UE_LOG(LogEANetworking, Log, TEXT("Create Server"));
// 	FOnlineSessionSettings SessionSettings;
// 	SessionSettings.bAllowJoinInProgress = true;
// 	SessionSettings.bIsDedicated=false;
// 	SessionSettings.bIsLANMatch=false;
// 	SessionSettings.bShouldAdvertise=true;
// 	SessionSettings.bUsesPresence=true;
// 	SessionSettings.NumPublicConnections=8;
// 	FOnlineSessionSetting EA_Id_Setting;
// 	FVariantData Id_Value = FString(EA_SESSION_MAIN_FILTER_VALUE);
// 	EA_Id_Setting = Id_Value;
// 	EA_Id_Setting.AdvertisementType=EOnlineDataAdvertisementType::ViaOnlineServiceAndPing;
// 	SessionSettings.Settings.Add(FName(EA_SESSION_MAIN_FILTER_KEY),EA_Id_Setting);
// 	
// 	SessionInterface->CreateSession(0,FName(EA_SESSION_NAME),SessionSettings);
// 	
// 	
// }

// void UEAGameInstance::JoinServer()
// {
// 	UE_LOG(LogEANetworking, Log, TEXT("Join Server"));
// 	SessionSearch = MakeShareable(new FOnlineSessionSearch());
// 	SessionSearch->bIsLanQuery = false;
// 	SessionSearch->MaxSearchResults=200000;
// 	
// 	FOnlineSearchSettings FilterSettings;
// 	FilterSettings.Set(EA_SESSION_MAIN_FILTER_KEY,EA_SESSION_MAIN_FILTER_VALUE,EOnlineComparisonOp::Equals);
// 	FilterSettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
// 	FilterSettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
// 	SessionSearch->QuerySettings = FilterSettings;
// 	SessionInterface->FindSessions(0,SessionSearch.ToSharedRef());
// }
