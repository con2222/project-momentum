

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/Player/Components/MomentumMovementComponent.h"
#include "MomentumAnimInstance.generated.h"

class AMomentumCharacter;

/**
 * 
 */
UCLASS()
class PROJECTMOMENTUM_API UMomentumAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|WallRun")
	bool bIsWallRunning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|WallRun")
	EWallRunSide WallRunSide;
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AMomentumCharacter> MomentumCharacter;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Direction;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;
	
	
};
