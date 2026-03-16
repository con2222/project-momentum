
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MomentumMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOMENTUM_API UMomentumMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	
	virtual float GetMaxSpeed() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Sprint")
	float SprintSpeedMultiplier = 1.5f;
};
