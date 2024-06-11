// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EAPlayerTagWidget.h"

void UEAPlayerTagWidget::SetPlayerTagProperties(FString Name, bool IsTeamA)
{
	PlayerName->SetText(FText::FromString(Name));
	FColor color = IsTeamA ? FColor(0,0,255) : FColor(255,0,0);
	PlayerName->SetColorAndOpacity(color);
}
