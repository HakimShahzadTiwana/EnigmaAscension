// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EAInteractionComponent.h"

#include "Interfaces/EAGameplayInterface.h"

// Sets default values for this component's properties
UEAInteractionComponent::UEAInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEAInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	AActor* Owner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	//FHitResult Hit;
	//GetWorld()->LineTraceSingleByObjectType(Hit,EyeLocation,End,ObjectQueryParams);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	float Radius = 30.0f;
	Shape.SetSphere(Radius);
	GetWorld()->SweepMultiByObjectType(Hits,EyeLocation,End,FQuat::Identity,ObjectQueryParams,Shape);

	for(auto Hit : Hits)
	{
		if(AActor* HitActor = Hit.GetActor())
		{
			if(HitActor->Implements<UEAGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(Owner);
				IEAGameplayInterface::Execute_Interact(HitActor,MyPawn);
				break;	
			}
		}
		DrawDebugSphere(GetWorld(),Hit.ImpactPoint,Radius,32,FColor::Red,false,2.0f);
	}
	DrawDebugLine(GetWorld(),EyeLocation,End,FColor::Black,false,1.0f,0,2.0f);	
}


// Called when the game starts
void UEAInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEAInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

