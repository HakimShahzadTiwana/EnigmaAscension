// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "LobbyPlayerEntry.generated.h"

/**
 * 
 */
UCLASS()
class ENIGMAASCENSION_API ULobbyPlayerEntry : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	UHorizontalBox* LobbyInfoCard;
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	UTextBlock* PlayerName;
public:
	UFUNCTION()
	void SetPlayerName(FString Name);
	
};
