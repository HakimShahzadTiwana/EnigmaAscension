// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

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
