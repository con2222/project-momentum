

#include "Character/Components/MomentumMovementComponent.h"

#include "Character/MomentumCharacter.h"

float UMomentumMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	
	AMomentumCharacter* MomentumCharacterOwner = Cast<AMomentumCharacter>(GetOwner());
	
	if (MomentumCharacterOwner && MomentumCharacterOwner->bWantsToSprint)
	{
		return MaxSpeed * SprintSpeedMultiplier;
	}
	
	return MaxSpeed;
}
