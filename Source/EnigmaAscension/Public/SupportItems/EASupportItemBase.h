// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/EAGameplayInterface.h"
#include "EASupportItemBase.generated.h"

class UGameplayEffect;
class USphereComponent;

UCLASS()
class ENIGMAASCENSION_API AEASupportItemBase : public AActor, public IEAGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEASupportItemBase();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category="GAS|Defaults")
	TSubclassOf<UGameplayEffect>HealEffect;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,Category="SupportItem")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;
	UFUNCTION()
	void ShowSupportItem();
	
	void HideAndCooldownSupportItem();

	void SetSupportItemState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	USphereComponent* SphereComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
