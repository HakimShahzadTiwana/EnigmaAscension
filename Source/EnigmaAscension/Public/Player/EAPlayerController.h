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
	int Frame = -1 ;
	// Owning Player Index
	UPROPERTY(BlueprintReadWrite)
	int InstigatorControllerID = -1;
};

USTRUCT(BlueprintType)
struct FPlayerInputData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FInputID PlayerInputID;
	
	UPROPERTY(BlueprintReadWrite)
	EEAAbilityInput InputType = EEAAbilityInput::None;

	UPROPERTY(BlueprintReadWrite)
	FVector InputValue = FVector::Zero();
	// Target Player Index
	UPROPERTY(BlueprintReadWrite)
	int TargetControllerID = -1 ;
	
	UPROPERTY(BlueprintReadWrite)
	float Timestamp = -1 ;
	
	UPROPERTY(BlueprintReadWrite)
	int ClientPing = -1 ;
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
	UPROPERTY(Replicated)
	bool bIsTeamA;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(EditAnywhere,Category="UI|PlayerHUD")
	TSubclassOf<UPlayerHUD> PlayerHUDClass;
	UPROPERTY(BlueprintReadWrite)
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
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool GetPlayerTeam(); 

	// Roll Back Net Code
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void Server_CollectInputData(FPlayerInputData Data);
	UFUNCTION(BlueprintCallable,Client, Reliable)
	void Client_UpdateTeamUI(const TArray<FString>& PlayerNames , const TArray<bool>& PlayerTeams);
	UFUNCTION(BlueprintCallable)
	FPlayerInputData Client_CollectInputData(EEAAbilityInput InputType,FVector InputValue = FVector::ZeroVector, int TargetControllerID = -1);
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(Client,Reliable)
	virtual void Client_UpdateHealthUI(float NewHealth,float MaxHealth);
	UFUNCTION(Client,Reliable)
	virtual void Client_UpdateStaminaUI(float NewStamina,float MaxStamina);
	UFUNCTION(Client,Reliable)
	virtual void Client_UpdateManaUI(float NewMana,float MaxMana);
	UFUNCTION(Client,Reliable)
	virtual void Client_TimerUI(int time);
	UFUNCTION(Client,Reliable)
	virtual void Client_CreateHUD();
	UFUNCTION(Client,Reliable,BlueprintCallable)
	virtual void Client_SetupPlayerCharacterUI(const FString &name, bool team);
	UFUNCTION(Server,Reliable,BlueprintCallable)
	virtual void Server_SetupPlayerCharacterUI(const FString &name, bool team);

	UFUNCTION()
	void Open_GameWonUI(bool bTeamAWon);
	UFUNCTION(NetMulticast,Reliable)
	void SetInputModeGameOnly();
};
