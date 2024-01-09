// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EACharactersEnum.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class UEACharacters : uint8
{
	Countess UMETA(DisplayName="Countess"),
	GreyStone UMETA(DisplayName="GreyStone"),
  };

UCLASS()
class ENIGMAASCENSION_API UEACharactersEnum : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
