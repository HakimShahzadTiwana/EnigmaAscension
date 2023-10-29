// Fill out your copyright notice in the Description page of Project Settings.


#include "SupportItems/EASupportItem_HealthItem.h"

#include "GameplayAbilitySystem/EAAbilitySystemComponent.h"
#include "GameplayAbilitySystem/EAAttributeSet.h"
#include "Player/EACharacter.h"

// Sets default values
AEASupportItem_HealthItem::AEASupportItem_HealthItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEASupportItem_HealthItem::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Blue,TEXT("Interacted"));
	if(!ensure(InstigatorPawn))
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("Interacted with support item - Health")));
	//TODO: Find a better place to keep the Apply HealAbility function
	if (AEACharacter* TargetCharacter = Cast<AEACharacter>(InstigatorPawn))
	{
		TargetCharacter->ApplyEffectToSelf(HealEffect);
		HideAndCooldownSupportItem();
	}
}



