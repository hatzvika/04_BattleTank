// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ATank* ControlledTank = GetControlledTank();
	if (ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("The controlled tank is %s."), *ControlledTank->GetName())
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No controlled tank found!"))
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation; // Out parameter
		if (GetSightRayHitLocation(HitLocation)) {
			GetControlledTank()->AimAt(HitLocation);
		}
	// If it hits the landscape
		//Tell controlled tank to aim at this point
}

// Get world location of linetrace through crosshair
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	// Find the crosshair position
	int32 ViewportSizeX = 0, ViewportSizeY = 0;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

	// "De-project" the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection)) {
		// Line trace along the look direction, and see what we hit (up to max range)
		GetLookVectorHitLocation(LookDirection, OutHitLocation);
		
	}
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; // To be discarded
	if (DeprojectScreenPositionToWorld(
			ScreenLocation.X,
			ScreenLocation.Y,
			CameraWorldLocation,
			LookDirection)
		)
	{
		return true;
	}
	else { return false; }
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector & OutHitLocation) const
{
	FHitResult HitResult;
	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	FVector EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_Visibility)
		) 
	{
		OutHitLocation = HitResult.Location;
		return true;
	}
	OutHitLocation = FVector(0.0f);
	return false;
}


