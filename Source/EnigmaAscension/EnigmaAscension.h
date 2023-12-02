// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EEAAbilityInput : uint8
{
	None UMETA(DisplayName = "None"),
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	Sprint UMETA(DisplayName = "Sprint"),
	Crouch UMETA(DisplayName = "Crouch"),
	PrimaryAttack UMETA(DisplayName = "PrimaryAttack"),
	SecondaryAttack UMETA(DisplayName = "SecondaryAttack"),
	SimpleAbility UMETA(DisplayName = "SimpleAbility"),
	UltimateAbility UMETA(DisplayName = "UltimateAbility")
};