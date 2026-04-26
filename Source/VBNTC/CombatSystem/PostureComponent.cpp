#include "PostureComponent.h"

UPostureComponent::UPostureComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPostureComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentPosture = 0.f;
}

void UPostureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Handle broken state timer
	if (bPostureBroken)
	{
		BrokenTimer -= DeltaTime;
		if (BrokenTimer <= 0.f)
		{
			ResetPosture();
		}
		return;
	}

	// Track time since last hit
	TimeSinceLastHit += DeltaTime;

	// Recover posture after delay
	if (TimeSinceLastHit >= RecoveryDelay && CurrentPosture > 0.f)
	{
		CurrentPosture = FMath::Max(0.f, CurrentPosture - RecoveryRate * DeltaTime);
		OnPostureChanged.Broadcast(GetPosturePercent());
	}
}

void UPostureComponent::AddPosture(float Amount)
{
	if (bPostureBroken || Amount <= 0.f) return;

	TimeSinceLastHit = 0.f;

	CurrentPosture = FMath::Min(MaxPosture, CurrentPosture + Amount);
	OnPostureChanged.Broadcast(GetPosturePercent());

	if (CurrentPosture >= MaxPosture)
	{
		BreakPosture();
	}
}

float UPostureComponent::GetPosturePercent() const
{
	return MaxPosture > 0.f ? CurrentPosture / MaxPosture : 0.f;
}

void UPostureComponent::NotifyHitEnded()
{
	TimeSinceLastHit = 0.f;
}

void UPostureComponent::BreakPosture()
{
	bPostureBroken = true;
	BrokenTimer = BrokenDuration;
	OnPostureBroken.Broadcast();
}

void UPostureComponent::ResetPosture()
{
	bPostureBroken = false;
	CurrentPosture = 0.f;
	BrokenTimer = 0.f;
	TimeSinceLastHit = 0.f;
	OnPostureChanged.Broadcast(0.f);
}
