// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

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
}

void UPlayerHUD::UpdateTeamScore(bool bIsTeamA, int Score)
{
	if(bIsTeamA)
	{
		TeamA_Score->SetText(FText::FromString(FString::FromInt(Score)));
		
	}
	else
	{
		TeamB_Score->SetText(FText::FromString(FString::FromInt(Score)));
	}

}
