// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!physicsHandle) 
		UE_LOG(LogTemp, Error, TEXT("Missing Physics Handle component on object %s"), *GetOwner()->GetName())
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector playerViewLoc;
	FRotator playerViewRot;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewLoc, playerViewRot);

	FVector lineTraceEnd = playerViewLoc + (playerViewRot.Vector() * reach);
	//UE_LOG(LogTemp, Warning, TEXT("Loc: %s ; Rot: %s"), *playerViewLoc.ToString(), *playerViewRot.ToString());

	DrawDebugLine(GetWorld(), playerViewLoc, lineTraceEnd, FColor(0, 255, 0), false, 0.0f, 0, 10.0f);

	// Cast out ray to find PhysicsBody objects.
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByObjectType(hitResult, playerViewLoc, lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner()));
	
	if (hitResult.GetActor())
		UE_LOG(LogTemp, Warning, TEXT("Trace hit: %s"), *hitResult.GetActor()->GetName())
}

