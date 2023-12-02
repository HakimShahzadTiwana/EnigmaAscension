// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnigmaAscension/EnigmaAscension.h"
#include "GameFramework/PlayerController.h"
#include "UI/PlayerHUD.h"
#include "EAPlayerController.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInputID
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	int Frame;
	// Owning Player Index
	UPROPERTY(BlueprintReadWrite)
	int InstigatorControllerID;
};

USTRUCT(BlueprintType)
struct FPlayerInputData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FInputID PlayerInputID;
	
	UPROPERTY(BlueprintReadWrite)
	EEAAbilityInput InputType;
	
	// Target Player Index
	UPROPERTY(BlueprintReadWrite)
	int TargetControllerID;
	
	UPROPERTY(BlueprintReadWrite)
	float Timestamp;
	
	UPROPERTY(BlueprintReadWrite)
	float ClientPing;
};

UCLASS()
class ENIGMAASCENSION_API AEAPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEAPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	void BindGasInputs();
	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY(EditAnywhere,Category="UI|PlayerHUD")
	TSubclassOf<UPlayerHUD> PlayerHUDClass;
	UPROPERTY()
	UPlayerHUD* PlayerHUD;

	
	UPROPERTY()
	int StartFrame;
	UPROPERTY()
	int CurrentFrame = 0;
	
	UFUNCTION()
	void MoveForward(float X);
	UFUNCTION()
	void MoveRight(float X);
	UFUNCTION()
	void Turn(float X);
	UFUNCTION()
	void LookUp(float X);
	UFUNCTION()
	void StartJump();
	UFUNCTION()
	void StopJump();
	UFUNCTION()
	void StartSprint();
	UFUNCTION()
	void StopSprint();

	// Roll Back Net Code
	UFUNCTION(Server, Reliable)
	void Server_CollectInputData(FPlayerInputData Data);
	UFUNCTION(BlueprintCallable)
	FPlayerInputData Client_CollectInputData(EEAAbilityInput InputType, int TargetControllerID);
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(Client,Reliable)
	virtual void Client_UpdateHealthUI(float NewHealth,float MaxHealth);

	UFUNCTION(Client,Reliable)
	virtual void Client_CreateHUD();
};
