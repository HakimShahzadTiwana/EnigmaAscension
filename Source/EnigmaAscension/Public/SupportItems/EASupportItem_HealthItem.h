// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EASupportItemBase.h"
#include "GameFramework/Actor.h"
#include "EASupportItem_HealthItem.generated.h"

UCLASS()
class ENIGMAASCENSION_API AEASupportItem_HealthItem : public AEASupportItemBase
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AEASupportItem_HealthItem();
	void Interact_Implementation(APawn* InstigatorPawn) override;
	

};
