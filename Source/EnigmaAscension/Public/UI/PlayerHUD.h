// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class ENIGMAASCENSION_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* StaminaBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* ManaBar;
	
	UFUNCTION()
	void SetHealth(float CurrentHealth, float MaxHealth);
	UFUNCTION()
	void SetStamina(float CurrentStamina, float MaxStamina);
	UFUNCTION()
	void SetMana(float CurrentMana, float MaxMana);
};
