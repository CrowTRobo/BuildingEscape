// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "Grabber.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();

	if (!pressurePlate)
		UE_LOG(LogTemp, Error, TEXT("pressurePlate missing from %s"), *owner->GetName())
}

void UOpenDoor::OpenDoor()
{
	OnOpenDoor.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the TriggerVolume
	if (pressurePlate && GetTotalMassOnPlate() >= triggerMass) {
		OpenDoor();
		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() - lastDoorOpenTime >= doorCloseDelay) {
		CloseDoor();
	}
}


// Calculates the total mass in kg currently on pressure plate
float UOpenDoor::GetTotalMassOnPlate() {

	TArray<AActor*> overlappingActors;

	if (pressurePlate)
		pressurePlate->GetOverlappingActors(overlappingActors);

	float totalMass = 0.0f;

	for (auto& actor : overlappingActors) {
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("Overlapping actor: %s total mass: %f"), *actor->GetName(), totalMass);
	}

	return totalMass;
}

