#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DirectionalAttackComponent.generated.h"

USTRUCT(BlueprintType)
struct FAttackDirection
{
	GENERATED_BODY()

	
	UPROPERTY(BlueprintReadOnly)
	float Angle = 0.f;


	UPROPERTY(BlueprintReadOnly)
	FVector2D Direction = FVector2D::ZeroVector;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackDirectionDetected, FAttackDirection, Direction);

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class VBNTC_API UDirectionalAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDirectionalAttackComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	UFUNCTION(BlueprintCallable, Category = "Directional Attack")
	void TriggerAttack();


	UFUNCTION(BlueprintCallable, Category = "Directional Attack")
	void AddMouseDelta(float DeltaX, float DeltaY);

	UFUNCTION(BlueprintPure, Category = "Directional Attack")
	FAttackDirection GetLastAttackDirection() const { return LastDirection; }

	
	UFUNCTION(BlueprintPure, Category = "Directional Attack")
	bool HasValidDirection() const { return bHasValidDirection; }

	// Fired when an attack is triggered
	UPROPERTY(BlueprintAssignable, Category = "Directional Attack")
	FOnAttackDirectionDetected OnAttackDirectionDetected;

protected:
	virtual void BeginPlay() override;

private:
	// How long before attack input to sample mouse movement (seconds)
	UPROPERTY(EditAnywhere, Category = "Directional Attack", meta = (ClampMin = "0.05", ClampMax = "0.5"))
	float DetectionWindow = 0.2f;

	// Minimum mouse movement magnitude to register a direction
	UPROPERTY(EditAnywhere, Category = "Directional Attack", meta = (ClampMin = "0.0"))
	float MinMouseDeltaThreshold = 5.f;

	// Accumulated mouse delta over the detection window
	FVector2D AccumulatedDelta = FVector2D::ZeroVector;

	FAttackDirection LastDirection;
	bool bHasValidDirection = false;

	FAttackDirection ResolveDirection(FVector2D Delta) const;
};
