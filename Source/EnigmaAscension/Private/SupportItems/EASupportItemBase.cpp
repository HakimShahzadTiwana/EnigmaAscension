// Fill out your copyright notice in the Description page of Project Settings.


#include "SupportItems/EASupportItemBase.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEASupportItemBase::AEASupportItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComponent->SetCollisionProfileName("SupportItem");
	RootComponent = SphereComponent;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	ItemMesh->SetupAttachment(SphereComponent);
	bReplicates = true;
	

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
	UE_LOG(LogCore, Log, TEXT("%hs - Showing support item"), __FUNCTION__);

	SetSupportItemState(true);
}

void AEASupportItemBase::HideAndCooldownSupportItem()
{
	UE_LOG(LogCore, Log, TEXT("%hs - Hiding Support item"), __FUNCTION__);

	SetSupportItemState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer,this,&AEASupportItemBase::ShowSupportItem,RespawnTime);
}

void AEASupportItemBase::SetSupportItemState(bool bNewIsActive)
{
	UE_LOG(LogCore, Log, TEXT("%hs - Setting support item state to %d"), __FUNCTION__,bNewIsActive);

	SetActorEnableCollision(bNewIsActive);
	RootComponent->SetVisibility(bNewIsActive,true);
}



// Called every frame
void AEASupportItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEASupportItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the mesh component
	DOREPLIFETIME(AEASupportItemBase, ItemMesh);
	DOREPLIFETIME(AEASupportItemBase,SphereComponent);
}
