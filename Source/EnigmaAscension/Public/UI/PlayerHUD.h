// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyPlayerEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class ENIGMAASCENSION_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULobbyPlayerEntry> LobbyPlayerEntry;
	// Lobby UI
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	UCanvasPanel* Canvas_Lobby;
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	UHorizontalBox* LobbyMessageContainer;
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	UVerticalBox* LobbyInfo;
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	UVerticalBox* LobbyInfoTeamA;
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	UVerticalBox* LobbyInfoTeamB;
	
	// Attribute UI
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	UCanvasPanel* Canvas_InGame;
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	UVerticalBox* AttributeContainer;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* StaminaBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* ManaBar;

	bool bIsLobbyListUpdated = false;
	UFUNCTION()
	void SetHealth(float CurrentHealth, float MaxHealth);
	UFUNCTION()
	void SetStamina(float CurrentStamina, float MaxStamina);
	UFUNCTION()
	void SetMana(float CurrentMana, float MaxMana);
	UFUNCTION(BlueprintCallable)
	void SetVisibility_Canvas_InGame(ESlateVisibility Visible);
	UFUNCTION(BlueprintCallable)
	void SetVisibility_Canvas_Lobby(ESlateVisibility Visible);
	UFUNCTION()
	void AddPlayer_LobbyInfo(TArray<FString> PlayerNames , TArray<bool> PlayerTeams);
};
