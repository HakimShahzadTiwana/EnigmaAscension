// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/EAGameplayInterface.h"
#include "Player/EACharacter.h"
#include "EASupportItemBase.generated.h"

class UGameplayEffect;
class USphereComponent;

UCLASS()
class ENIGMAASCENSION_API AEASupportItemBase : public AActor, public IEAGameplayInterface
{
	GENERATED_BODY()
	
public:
	
	AEASupportItemBase();

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category="GAS|Defaults")
	TSubclassOf<UGameplayEffect>HealEffect;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	void HideAndCooldownSupportItem();
	
protected:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components",Replicated)
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components",Replicated)
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(EditAnywhere,Category="SupportItem")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	
	UFUNCTION()
	void ShowSupportItem();
	
	void SetSupportItemState(bool bNewIsActive);


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
