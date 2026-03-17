
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MomentumMovementComponent.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWallRunSide : uint8
{
	None,
	Left,
	Right
};

UCLASS()
class PROJECTMOMENTUM_API UMomentumMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	
	virtual float GetMaxSpeed() const override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Movement|WallRun")
	void TryWallRun();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Sprint")
	float SprintSpeedMultiplier = 1.5f;
	
	UFUNCTION(BlueprintCallable, Category = "Movement|WallRun")
	void StopWallRun();
	
	UFUNCTION(BlueprintCallable, Category = "Movement|WallRun")
	bool IsWallRunning() const { return bIsWallRunning; }
	
	UFUNCTION(BlueprintCallable, Category = "Movement|WallRun")
	void DoWallJump();
	
	UFUNCTION(BlueprintCallable, Category = "Movement|WallRun")
	EWallRunSide GetCurrentWallSide() const { return CurrentWallSide; }
	
protected:
	
	bool WallCheck(FHitResult& WallHit, bool bLeft);
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement|WallRun")
	float WallJumpForce = 700.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement|WallRun")
	float WallJumpVerticalForce = 400.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement|WallRun")
	float WallCheckDistance = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement|WallRun")
	float MaxWallRunTime = 2.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement|WallRun")
	float MaxWallRunSpeed = 800.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|WallRun")
	bool bIsWallRunning = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|WallRun")
	EWallRunSide CurrentWallSide = EWallRunSide::None;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|WallRun")
	FVector CurrentWallNormal;
	
	FTimerHandle WallRunTimer;
	
	void StartWallRun(EWallRunSide Side, const FVector& WallNormal);
};
