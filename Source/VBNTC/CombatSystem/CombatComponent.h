#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Idle,
	Attacking,
	Blocking,
	Staggered
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitLanded, AActor*, HitActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeflect);

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class VBNTC_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Call from Attack animation notify to activate the hitbox window
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartAttackTrace();

	// Call from animation notify when the swing window ends
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndAttackTrace();

	// Call when the owner wants to block (e.g. input held)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartBlocking();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopBlocking();

	// Returns true if currently in a block state
	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsBlocking() const { return CombatState == ECombatState::Blocking; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	ECombatState GetCombatState() const { return CombatState; }

	// Set the weapon mesh used for sweep traces
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetWeaponMesh(UPrimitiveComponent* Weapon);

	// Events
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnHitLanded OnHitLanded;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnDeflect OnDeflect;

protected:
	virtual void BeginPlay() override;

private:
	// Damage dealt on a clean (unblocked) hit
	UPROPERTY(EditAnywhere, Category = "Combat")
	float HitDamage = 20.f;

	// Posture damage dealt to blocker
	UPROPERTY(EditAnywhere, Category = "Combat")
	float BlockPostureDamage = 30.f;

	// Posture damage dealt on a clean hit
	UPROPERTY(EditAnywhere, Category = "Combat")
	float HitPostureDamage = 15.f;

	// Deflect window in seconds at the start of a block
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeflectWindow = 0.2f;

	// Radius of the sphere sweep for hit detection
	UPROPERTY(EditAnywhere, Category = "Combat")
	float TraceRadius = 20.f;

	ECombatState CombatState = ECombatState::Idle;

	bool bTraceActive = false;
	float BlockStartTime = 0.f;

	// Actors already hit this swing (prevents multi-hit per swing)
	TArray<AActor*> HitActorsThisSwing;

	UPROPERTY()
	UPrimitiveComponent* WeaponMesh = nullptr;

	FVector LastWeaponLocation = FVector::ZeroVector;

	void PerformHitTrace();
	void ApplyHitToActor(AActor* HitActor, bool bWasBlocked);
};
