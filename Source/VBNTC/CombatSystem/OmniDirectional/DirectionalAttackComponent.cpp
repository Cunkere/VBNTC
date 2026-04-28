#include "DirectionalAttackComponent.h"

UDirectionalAttackComponent::UDirectionalAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDirectionalAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDirectionalAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Smoothly decay accumulated delta so old input fades out
	AccumulatedDelta *= FMath::Max(0.f, 1.f - DeltaTime / DetectionWindow);
}

void UDirectionalAttackComponent::AddMouseDelta(float DeltaX, float DeltaY)
{
	AccumulatedDelta += FVector2D(DeltaX, DeltaY);
}

void UDirectionalAttackComponent::TriggerAttack()
{
	if (AccumulatedDelta.Size() >= MinMouseDeltaThreshold)
	{
		LastDirection = ResolveDirection(AccumulatedDelta);
		bHasValidDirection = true;
	}
	else
	{
		// Not enough mouse movement — neutral/default attack
		LastDirection = FAttackDirection();
		bHasValidDirection = false;
	}

	// Reset so next attack starts fresh
	AccumulatedDelta = FVector2D::ZeroVector;

	OnAttackDirectionDetected.Broadcast(LastDirection);
}

FAttackDirection UDirectionalAttackComponent::ResolveDirection(FVector2D Delta) const
{
	FAttackDirection Result;

	// Normalize the delta to get a clean direction vector
	Result.Direction = Delta.GetSafeNormal();

	// Convert to angle:
	// Atan2 gives angle from +X axis, we want angle from +Y (up on screen)
	// Screen space: X = right, Y = down (so we flip Y for intuitive up = forward)
	// Result is 0-360 clockwise from up
	float RadAngle = FMath::Atan2(Delta.X, -Delta.Y);
	float DegAngle = FMath::RadiansToDegrees(RadAngle);

	// Normalize to 0-360
	if (DegAngle < 0.f)
	{
		DegAngle += 360.f;
	}

	Result.Angle = DegAngle;

	return Result;
}