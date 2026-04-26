#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PostureComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPostureBroken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPostureChanged, float, NewPosturePercent);

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class VBNTC_API UPostureComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPostureComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Add posture damage (called on block or hit)
	UFUNCTION(BlueprintCallable, Category = "Posture")
	void AddPosture(float Amount);

	// Returns current posture as 0.0 - 1.0 percentage
	UFUNCTION(BlueprintPure, Category = "Posture")
	float GetPosturePercent() const;

	UFUNCTION(BlueprintPure, Category = "Posture")
	bool IsPostureBroken() const { return bPostureBroken; }

	// Call this to signal the owner stopped being hit (starts recovery delay)
	UFUNCTION(BlueprintCallable, Category = "Posture")
	void NotifyHitEnded();

	// Events
	UPROPERTY(BlueprintAssignable, Category = "Posture")
	FOnPostureBroken OnPostureBroken;

	UPROPERTY(BlueprintAssignable, Category = "Posture")
	FOnPostureChanged OnPostureChanged;

protected:
	virtual void BeginPlay() override;

private:
	// Max posture before breaking
	UPROPERTY(EditAnywhere, Category = "Posture", meta = (ClampMin = "1.0"))
	float MaxPosture = 100.f;

	// How much posture drains per second when idle
	UPROPERTY(EditAnywhere, Category = "Posture", meta = (ClampMin = "0.0"))
	float RecoveryRate = 25.f;

	// Seconds after last hit before recovery begins
	UPROPERTY(EditAnywhere, Category = "Posture", meta = (ClampMin = "0.0"))
	float RecoveryDelay = 1.5f;

	// How long posture break stun lasts
	UPROPERTY(EditAnywhere, Category = "Posture", meta = (ClampMin = "0.0"))
	float BrokenDuration = 2.5f;

	float CurrentPosture = 0.f;
	float TimeSinceLastHit = 0.f;
	float BrokenTimer = 0.f;
	bool bPostureBroken = false;
	bool bRecovering = false;

	void BreakPosture();
	void ResetPosture();
};
