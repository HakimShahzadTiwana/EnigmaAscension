// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "Core/EAGameMode.h"
#include "Core/EAGameState.h"
#include "Kismet/GameplayStatics.h"

void UPlayerHUD::SetHealth(float CurrentHealth, float MaxHealth)
{
	if(IsValid(HealthBar)){
		HealthBar->SetPercent(CurrentHealth/MaxHealth);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("PlayerHUD: HealthBar Is Not Valid"));
	}
}

void UPlayerHUD::SetStamina(float CurrentStamina, float MaxStamina)
{
	if(IsValid(StaminaBar)){
		StaminaBar->SetPercent(CurrentStamina/MaxStamina);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("PlayerHUD: StaminaBar Is Not Valid"));
	}
}

void UPlayerHUD::SetMana(float CurrentMana, float MaxMana)
{
	if(IsValid(ManaBar)){
		ManaBar->SetPercent(CurrentMana/MaxMana);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("PlayerHUD: ManaBar Is Not Valid"));
	}
}

void UPlayerHUD::SetVisibility_Canvas_InGame(ESlateVisibility Visible)
{
	Canvas_InGame->SetVisibility(Visible);
}

void UPlayerHUD::SetVisibility_Canvas_Lobby(ESlateVisibility Visible)
{
	Canvas_Lobby->SetVisibility(Visible);
}

void UPlayerHUD::AddPlayer_LobbyInfo(TArray<FString> PlayerNames , TArray<bool> PlayerTeams)
{
	
	if(bIsLobbyListUpdated)
	{
		UUserWidget* temp = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0),LobbyPlayerEntry);
		ULobbyPlayerEntry* PlayerEntry = Cast<ULobbyPlayerEntry>(temp);
		PlayerEntry->SetPlayerName(PlayerNames[PlayerNames.Num()-1]);
		
		if(PlayerTeams[PlayerTeams.Num()-1]){
			LobbyInfoTeamA->AddChildToVerticalBox(PlayerEntry);
		}
		else{
			LobbyInfoTeamB->AddChildToVerticalBox(PlayerEntry);
		}
	}
	else
	{
		bIsLobbyListUpdated=true;
		for (int i = 0 ; i<PlayerNames.Num();i++)
		{
			UUserWidget* temp = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0),LobbyPlayerEntry);
			ULobbyPlayerEntry* PlayerEntry = Cast<ULobbyPlayerEntry>(temp);
			PlayerEntry->SetPlayerName(PlayerNames[i]);
		
			if(PlayerTeams[i]){
				LobbyInfoTeamA->AddChildToVerticalBox(PlayerEntry);
			}
			else{
				LobbyInfoTeamB->AddChildToVerticalBox(PlayerEntry);
			}
		}
	}
	

	
}

void UPlayerHUD::HostStartGame()
{
	Canvas_InGame->SetVisibility(ESlateVisibility::Visible);
	Canvas_Lobby->SetVisibility(ESlateVisibility::Collapsed);
	UGameplayStatics::GetPlayerController(GetWorld(),0)->SetShowMouseCursor(false);
	FInputModeGameOnly GameOnly;
	UGameplayStatics::GetPlayerController(GetWorld(),0)->SetInputMode(GameOnly);
}

void UPlayerHUD::UpdateTimer_Implementation(int time)
{
	UE_LOG(LogCore, Log, TEXT("%s - Updating UI for Timer"), *FString(__FUNCTION__));
    
	// Calculate minutes and seconds
	int minutes = time / 60;
	int seconds = time % 60;
	FString final_text = minutes + ":" + seconds;
	// Update UI text
	Timer_Text->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), minutes, seconds)));
}


void UPlayerHUD::UpdateTeamScore_Implementation(bool bIsTeamA, int Score)
{
	if(bIsTeamA)
	{
		UE_LOG(LogCore, Log, TEXT("%hs - Updating UI for Team A"), __FUNCTION__);

		TeamA_Score->SetText(FText::FromString(FString::FromInt(Score)));
		
	}
	else
	{
		UE_LOG(LogCore, Log, TEXT("%hs - Updating UI for Team B"), __FUNCTION__);
		TeamB_Score->SetText(FText::FromString(FString::FromInt(Score)));
	}

}
