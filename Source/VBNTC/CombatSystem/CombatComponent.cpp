#include "Engine/DamageEvents.h"
#include "CombatComponent.h"
#include "PostureComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bTraceActive)
	{
		PerformHitTrace();
	}
}

void UCombatComponent::SetWeaponMesh(UPrimitiveComponent* Weapon)
{
	WeaponMesh = Weapon;
	if (WeaponMesh)
	{
		LastWeaponLocation = WeaponMesh->GetComponentLocation();
	}
}

void UCombatComponent::StartAttackTrace()
{
	if (!WeaponMesh) return;

	bTraceActive = true;
	HitActorsThisSwing.Empty();
	CombatState = ECombatState::Attacking;
	LastWeaponLocation = WeaponMesh->GetComponentLocation();
}

void UCombatComponent::EndAttackTrace()
{
	bTraceActive = false;
	HitActorsThisSwing.Empty();

	if (CombatState == ECombatState::Attacking)
	{
		CombatState = ECombatState::Idle;
	}
}

void UCombatComponent::StartBlocking()
{
	if (CombatState == ECombatState::Attacking) return;

	CombatState = ECombatState::Blocking;
	BlockStartTime = GetWorld()->GetTimeSeconds();
}

void UCombatComponent::StopBlocking()
{
	if (CombatState == ECombatState::Blocking)
	{
		CombatState = ECombatState::Idle;
	}
}

void UCombatComponent::PerformHitTrace()
{
	if (!WeaponMesh || !GetOwner()) return;

	FVector CurrentWeaponLocation = WeaponMesh->GetComponentLocation();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		LastWeaponLocation,
		CurrentWeaponLocation,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(TraceRadius),
		Params
	);

	// Debug visualization — remove for shipping
#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), CurrentWeaponLocation, TraceRadius, 8, bHit ? FColor::Red : FColor::Green, false, 0.1f);
#endif

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor || HitActorsThisSwing.Contains(HitActor)) continue;

			HitActorsThisSwing.Add(HitActor);
			ApplyHitToActor(HitActor, false);
		}
	}

	LastWeaponLocation = CurrentWeaponLocation;
}

void UCombatComponent::ApplyHitToActor(AActor* HitActor, bool bWasBlocked)
{
	if (!HitActor) return;

	// Check if the hit actor is blocking
	UCombatComponent* TargetCombat = HitActor->FindComponentByClass<UCombatComponent>();
	UPostureComponent* TargetPosture = HitActor->FindComponentByClass<UPostureComponent>();

	bool bTargetBlocking = TargetCombat && TargetCombat->IsBlocking();

	if (bTargetBlocking)
	{
		// Check for deflect window (perfect block)
		float TimeSinceBlockStart = GetWorld()->GetTimeSeconds() - TargetCombat->BlockStartTime;
		bool bDeflected = TimeSinceBlockStart <= DeflectWindow;

		if (bDeflected)
		{
			// Deflect — attacker gets posture damage instead
			UPostureComponent* AttackerPosture = GetOwner()->FindComponentByClass<UPostureComponent>();
			if (AttackerPosture)
			{
				AttackerPosture->AddPosture(BlockPostureDamage * 1.5f);
			}
			OnDeflect.Broadcast();
		}
		else
		{
			// Normal block — defender takes posture damage
			if (TargetPosture)
			{
				TargetPosture->AddPosture(BlockPostureDamage);
			}
		}
	}
	else
	{
		// Clean hit — deal damage and some posture
		ACharacter* HitChar = Cast<ACharacter>(HitActor);
		if (HitChar)
		{
			// Apply UE damage system — hook into your health component or ACharacter::TakeDamage
			HitActor->TakeDamage(HitDamage, FDamageEvent(), GetOwner()->GetInstigatorController(), GetOwner());
		}

		if (TargetPosture)
		{
			TargetPosture->AddPosture(HitPostureDamage);
		}

		OnHitLanded.Broadcast(HitActor);
	}
}
