// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/EAGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogEANetworking);
UEAGameInstance::UEAGameInstance()
{
}

void UEAGameInstance::OnFindSessionCompleted(bool Succeeded)
{
	UE_LOG(LogEANetworking, Log, TEXT("OnFindSessionCompleted"));
	if(Succeeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		UE_LOG(LogEANetworking, Log, TEXT("Find Session Succeeded, found %d Sessions"),SearchResults.Num());
		if(SearchResults.Num()>0)
		{
			UE_LOG(LogEANetworking, Log, TEXT("Joining Server..."));
			SessionInterface->JoinSession(0,"My Session",SearchResults[0]);
		}
	}
	else
	{
		UE_LOG(LogEANetworking, Warning, TEXT("Find Session Failed"));
	}
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
	SessionSettings.bIsLANMatch=true;
	SessionSettings.bShouldAdvertise=true;
	SessionSettings.bUsesPresence=true;
	SessionSettings.NumPublicConnections=8;
	
	SessionInterface->CreateSession(0,FName("My Session"),SessionSettings);
	
	
}

void UEAGameInstance::JoinServer()
{
	UE_LOG(LogEANetworking, Log, TEXT("Join Server"));
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults=10000;
	SessionSearch->QuerySettings.Set(FName(TEXT("PRESENCESEARCH")),true,EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0,SessionSearch.ToSharedRef());
}
