// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyPlayerEntry.h"

void ULobbyPlayerEntry::SetPlayerName(FString Name)
{
	PlayerName->SetText(FText::FromString(Name));
}
