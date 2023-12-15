// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EADefaultPawn.h"

// Sets default values
AEADefaultPawn::AEADefaultPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEADefaultPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEADefaultPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEADefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

