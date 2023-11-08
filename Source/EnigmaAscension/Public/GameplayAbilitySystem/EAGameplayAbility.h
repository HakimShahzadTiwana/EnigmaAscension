// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EnigmaAscension/EnigmaAscension.h"
#include "EAGameplayAbility.generated.h"

/**
 * 
 */

UCLASS()
class ENIGMAASCENSION_API UEAGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UEAGameplayAbility();

	//Define key binds for game abilities in blueprint by selecting the appropriate Ability Input
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AbilityID")
	EEAAbilityInput AbilityInputID=EEAAbilityInput::None;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AbilityID")		
	EEAAbilityInput AbilityID=EEAAbilityInput::None;


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffect;

	// TODO: WIP for Game Ability Implementation in C++ (Not working currently)
	UFUNCTION(BlueprintCallable)
	void OnAttackEventReceived(FGameplayEventData Payload);
	UFUNCTION(BlueprintCallable)
	void ActivateAttackGameplayEvent(UAnimMontage* MontageToPlay, FGameplayTag EventTag);
	
};
