// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "EAPlayerTagWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENIGMAASCENSION_API UEAPlayerTagWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,meta=(BindWidget),BlueprintReadWrite)
	UTextBlock* PlayerName;
	UPROPERTY(BlueprintReadWrite)
	bool bIsTeamA;
	
	//UFUNCTION(Client, Reliable)
	//void SetPlayerTagProperties(const FString& Name, bool IsTeamA);
	void SetPlayerTagProperties(FString Name, bool IsTeamA);
};
