


#include "Character/Animation/MomentumAnimInstance.h"
#include "Character/MomentumCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMomentumAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	MomentumCharacter = Cast<AMomentumCharacter>(TryGetPawnOwner());
}

void UMomentumAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (MomentumCharacter == nullptr)
	{
		MomentumCharacter = Cast<AMomentumCharacter>(TryGetPawnOwner());
	}
	
	if (MomentumCharacter)
	{
		if (UMomentumMovementComponent* MomentumMovement = Cast<UMomentumMovementComponent>(MomentumCharacter->GetCharacterMovement()))
		{
			FVector Velocity = MomentumCharacter->GetVelocity();
			Velocity.Z = 0.f;
			Speed = Velocity.Size();
		
			bIsFalling = MomentumMovement->IsFalling();
			
			bIsWallRunning = MomentumMovement->IsWallRunning();
			WallRunSide = MomentumMovement->GetCurrentWallSide();
		}
	}
}
