// Fill out your copyright notice in the Description page of Project Settings.


#include "SupportItems/EASupportItemBase.h"

#include "Components/SphereComponent.h"

// Sets default values
AEASupportItemBase::AEASupportItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComponent->SetCollisionProfileName("SupportItem");
	RootComponent = SphereComponent;

	RespawnTime=10.0f;
}

void AEASupportItemBase::Interact_Implementation(APawn* InstigatorPawn)
{

}

// Called when the game starts or when spawned
void AEASupportItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEASupportItemBase::ShowSupportItem()
{
	SetSupportItemState(true);
}

void AEASupportItemBase::HideAndCooldownSupportItem()
{
	SetSupportItemState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer,this,&AEASupportItemBase::ShowSupportItem,RespawnTime);
}

void AEASupportItemBase::SetSupportItemState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	RootComponent->SetVisibility(bNewIsActive,true);
}


// Called every frame
void AEASupportItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

