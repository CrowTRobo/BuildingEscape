// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber() {
	
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay() {
	
	Super::BeginPlay();

	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	// Grabber component must have a PhysicsHandle component attached as well
	if (!physicsHandle)
		UE_LOG(LogTemp, Error, TEXT("Missing Physics Handle component on object %s"), *GetOwner()->GetName())

		// Bind the input actions
		if (inputComponent) {
			UE_LOG(LogTemp, Warning, TEXT("InputComponent found on %s"), *GetOwner()->GetName());
			inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
			inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
		}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Move grabbed component to end of reach
	if (physicsHandle->GrabbedComponent) {
		FVector playerViewLoc;
		FRotator playerViewRot;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewLoc, playerViewRot);

		FVector lineTraceEnd = playerViewLoc + (playerViewRot.Vector() * reach);

		physicsHandle->SetTargetLocation(lineTraceEnd);
	}
}

// Ray-cast and grab object within reach
void UGrabber::Grab() {
	
	UE_LOG(LogTemp, Warning, TEXT("Grab action pressed"))

	FVector playerViewLoc;
	FRotator playerViewRot;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewLoc, playerViewRot);

	FVector lineTraceEnd = playerViewLoc + (playerViewRot.Vector() * reach);
	//UE_LOG(LogTemp, Warning, TEXT("Loc: %s ; Rot: %s"), *playerViewLoc.ToString(), *playerViewRot.ToString());

	//DrawDebugLine(GetWorld(), playerViewLoc, lineTraceEnd, FColor(0, 255, 0), false, 0.0f, 0, 10.0f);

	// Cast out ray to find PhysicsBody objects.
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByObjectType(hitResult, playerViewLoc, lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner()));

	UPrimitiveComponent *componentToGrab = hitResult.GetComponent();

	// Grab component if trace hit an actor
	if (hitResult.GetActor()) {
		UE_LOG(LogTemp, Warning, TEXT("Trace hit: %s"), *hitResult.GetActor()->GetName())

		physicsHandle->GrabComponentAtLocationWithRotation(componentToGrab, NAME_None, componentToGrab->GetOwner()->GetActorLocation(), componentToGrab->GetOwner()->GetActorRotation());
	}
}

// Release the object that has been grabbed
void UGrabber::Release() {
	
	UE_LOG(LogTemp, Warning, TEXT("Release action pressed"))

	// Release PhysicsHandle
	physicsHandle->ReleaseComponent();
}

