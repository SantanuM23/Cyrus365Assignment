// Copyright Epic Games, Inc. All Rights Reserved.

#include "Cyrus365AssignmentPickUpComponent.h"

UCyrus365AssignmentPickUpComponent::UCyrus365AssignmentPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UCyrus365AssignmentPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UCyrus365AssignmentPickUpComponent::OnSphereBeginOverlap);
}

void UCyrus365AssignmentPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ACyrus365AssignmentCharacter* Character = Cast<ACyrus365AssignmentCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
