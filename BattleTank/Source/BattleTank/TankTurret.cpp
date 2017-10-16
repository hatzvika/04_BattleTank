// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"
#include "Engine/World.h"

void UTankTurret::RotateTurret(float RelativeSpeed)
{
	// Move the turret the right amount this frame
	// Given a max speed and frame time
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	float RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	float NewRotation = RelativeRotation.Yaw + RotationChange;
	
	UE_LOG(LogTemp, Warning, TEXT("Rotation is: %f"), NewRotation)

	SetRelativeRotation(FRotator(0, NewRotation, 0));
}



