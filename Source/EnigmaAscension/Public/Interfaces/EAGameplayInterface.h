// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EAGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEAGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ENIGMAASCENSION_API IEAGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Interact function to implement for actors using this interface, can be implemented in code or blueprints
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
};
