// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EEAAbilityInput : uint8
{
	None,
	Confirm,
	Cancel,
	Sprint,
	Crouch,
	PrimaryAttack,
	SecondaryAttack,
	SimpleAbility,
	UltimateAbility
};