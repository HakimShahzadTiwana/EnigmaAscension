// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EAPlayerController.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "EnigmaAscension/EnigmaAscension.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "GameplayAbilitySystem/EAAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/EACharacter.h"
#include "Player/EAPlayerState.h"

AEAPlayerController::AEAPlayerController()
{
}

void AEAPlayerController::BeginPlay()
{
	UE_LOG(LogCore, Log, TEXT("AEAPlayerController::BeginPlay"));
	Super::BeginPlay();
	PlayerPawn = GetPawn();
	//Binding Gas Abilities here because InputComponent is called before begin play and Pawn is not valid there
	BindGasInputs();
}

void AEAPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CurrentFrame++;
}

void AEAPlayerController::SetupInputComponent()
{
	UE_LOG(LogCore, Log, TEXT("AEAPlayerController::SetupInputComponent"));
	Super::SetupInputComponent();
	if(!IsValid(InputComponent))
	{
		UE_LOG(LogCore, Warning, TEXT("InputComponent is not valid"));
		return;
	}
	
		UE_LOG(LogCore, Log, TEXT("AEAPlayerController::SetupInputComponent - Binding Inputs"));
		//Axis Mappings
		InputComponent->BindAxis("MoveForward",this,&AEAPlayerController::MoveForward);
		InputComponent->BindAxis("MoveRight",this,&AEAPlayerController::MoveRight);
		InputComponent->BindAxis("Turn",this,&AEAPlayerController::Turn);
		InputComponent->BindAxis("LookUp",this,&AEAPlayerController::LookUp);
		//ActionMappings
		InputComponent->BindAction("Jump",IE_Pressed,this,&AEAPlayerController::StartJump);
		InputComponent->BindAction("Jump",IE_Released,this,&AEAPlayerController::StopJump);
		
	
	
	
}

void AEAPlayerController::BindGasInputs() 
{
	if(IsValid(PlayerPawn))
	{
		if(AEACharacter* PlayerCharacter = Cast<AEACharacter>(PlayerPawn))
		{
			if(IsValid(PlayerCharacter->GetAbilitySystemComponent()))
			{
				const FGameplayAbilityInputBinds Binds("Confirm","Cancel",FTopLevelAssetPath(GetPathNameSafe(UClass::TryFindTypeSlow<UEnum>("EEAAbilityInput"))),static_cast<int32>(EEAAbilityInput::Confirm),static_cast<int32>(EEAAbilityInput::Cancel));
				PlayerCharacter->GetAbilitySystemComponent()->BindAbilityActivationToInputComponent(InputComponent,Binds);
				UE_LOG(LogCore,Log,TEXT("AEAPlayerController::BindGasInputs - Inputs have been binded"));
			}
			else
			{

				UE_LOG(LogCore,Warning,TEXT("AEAPlayerController::BindGasInputs - AbilitySystemComponent is null"));
			}
		}
		else
		{
			UE_LOG(LogCore, Warning, TEXT("AEAPlayerController::BindGasInputs - PlayerCharacter cast failed"));
		}
	}
	else
	{
		UE_LOG(LogCore, Warning, TEXT("AEAPlayerController::BindGasInputs - PlayerPawn is not valid"));
	}
	
}

void AEAPlayerController::MoveForward(float X)
{
	//UE_LOG(LogCore, Log, TEXT("AEAPlayerController::MoveForward"));
	if(IsValid(PlayerPawn))
	{
		FRotator ForwardRot =FRotator(0,PlayerPawn->GetControlRotation().Yaw,0);
		FVector ForwardVec = UKismetMathLibrary::GetForwardVector(ForwardRot);
		PlayerPawn->AddMovementInput(ForwardVec,X);
		
	}
	else
	{
		UE_LOG(LogCore, Warning, TEXT("AEAPlayerController::MoveForward - Player Pawn is not valid"));
	}
}

void AEAPlayerController::MoveRight(float X)
{
	//UE_LOG(LogCore, Log, TEXT("AEAPlayerController::MoveRight"));
	if(IsValid(PlayerPawn))
	{
		FRotator RightRot =FRotator(0,PlayerPawn->GetControlRotation().Yaw,0);
		FVector RightVec = UKismetMathLibrary::GetRightVector(RightRot);
		PlayerPawn->AddMovementInput(RightVec,X);
	}
	else
	{
		UE_LOG(LogCore, Warning, TEXT("AEAPlayerController::MoveRight - Player Pawn is not valid"));
	}
}

void AEAPlayerController::Turn(float X)
{
	//UE_LOG(LogCore, Log, TEXT("AEAPlayerController::Turn"));
	if(IsValid(PlayerPawn))
	{
		PlayerPawn->AddControllerYawInput(X);
	}
	else
	{
		UE_LOG(LogCore, Warning, TEXT("AEAPlayerController::Turn - Player Pawn is not valid"));
	}
}

void AEAPlayerController::LookUp(float X)
{
	//UE_LOG(LogCore, Log, TEXT(" AEAPlayerController::LookUp"));
	if(IsValid(PlayerPawn))
	{
		PlayerPawn->AddControllerPitchInput(X);
	}
	else
	{
		UE_LOG(LogCore, Warning, TEXT(" AEAPlayerController::LookUp - Player Pawn is not valid"));
	}
}

void AEAPlayerController::StartJump()
{
	UE_LOG(LogCore, Log, TEXT("AEAPlayerController::StartJump"));
	if(IsValid(GetCharacter()))
	{
		GetCharacter()->Jump();
	}
	else
	{
		UE_LOG(LogCore, Warning, TEXT("AEAPlayerController::StartJump - Character is not valid"));
	}
	
}

void AEAPlayerController::StopJump()
{
	UE_LOG(LogCore, Log, TEXT("AEAPlayerController::StopJump"));
	if(IsValid(GetCharacter()))
	{
		GetCharacter()->StopJumping();
	}
	else
	{
		UE_LOG(LogCore, Warning, TEXT("AEAPlayerController::StopJump - Character is not valid"));
	}
}

void AEAPlayerController::StartSprint()
{
}

void AEAPlayerController::StopSprint()
{
}

void AEAPlayerController::Server_CollectInputData_Implementation(FPlayerInputData Data)
{
}

FPlayerInputData AEAPlayerController::Client_CollectInputData(EEAAbilityInput InputType,int TargetControllerID)
{
	
	FInputID I_ID;

	I_ID.Frame = CurrentFrame; 
	I_ID.InstigatorControllerID = GetPlayerState<AEAPlayerState>()->MyPlayerIndex;

	FPlayerInputData PI_Data;
	
	PI_Data.PlayerInputID = I_ID;
	PI_Data.InputType = InputType; 
	PI_Data.TargetControllerID = TargetControllerID;
	
	PI_Data.Timestamp = GetWorld()->GetUnpausedTimeSeconds();
	PI_Data.ClientPing = UGameplayStatics::GetPlayerState(GetWorld(),0)->GetPingInMilliseconds();

	Server_CollectInputData_Implementation(PI_Data);
	
	return PI_Data;
}

void AEAPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UE_LOG(LogTemp,Warning,TEXT("AEAPlayerController::OnPossess"));
}

void AEAPlayerController::Client_CreateHUD_Implementation()
{
	if(IsLocalController()){
		UE_LOG(LogTemp,Warning,TEXT("AEAPlayerController::Client_CreateHUD_Implementation Player HUD Created"));
		PlayerHUD = CreateWidget<UPlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(),0),PlayerHUDClass);
		PlayerHUD->AddToViewport();
	}
}

void AEAPlayerController::Client_UpdateHealthUI_Implementation(float NewHealth,float MaxHealth)
{
	if(IsValid(PlayerHUD)){
		PlayerHUD->SetHealth(NewHealth,MaxHealth);
	}
	UE_LOG(LogGAS,Log,TEXT("AEACharacter::Client_UpdateHealthUI_Implementation - Updating Player UI New Health = %f, Max Health = %f"),
		 NewHealth,MaxHealth);
}
