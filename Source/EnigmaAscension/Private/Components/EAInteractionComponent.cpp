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
	//Create Collision Query Parameters
	FCollisionObjectQueryParams ObjectQueryParams;
	//Register collisions with only World Dynamic Types
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	//Get locations of eyes of the player character
	AActor* Owner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

	//Calculate how far from the eye location do we want to check for collisions
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	
	TArray<FHitResult> Hits;
	//Set sphere collision trace
	FCollisionShape Shape;
	float Radius = 30.0f;
	Shape.SetSphere(Radius);

	//Detect collision hits with objects from eye location to end point
	GetWorld()->SweepMultiByObjectType(Hits,EyeLocation,End,FQuat::Identity,ObjectQueryParams,Shape);
	
	for(auto Hit : Hits)
	{
		if(AActor* HitActor = Hit.GetActor())
		{
			if(HitActor->Implements<UEAGameplayInterface>())
			{
				//If hit actor implements the Gameplay interface then execute the interact implementation
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

