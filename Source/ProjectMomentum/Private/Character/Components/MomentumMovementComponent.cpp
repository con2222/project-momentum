

#include "Character/Components/MomentumMovementComponent.h"

#include "MomentumGameplayTags.h"
#include "Character/MomentumCharacter.h"

UMomentumMovementComponent::UMomentumMovementComponent()
{
	bWantsInitializeComponent = true;
}

void UMomentumMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	MomentumCharacterOwner = Cast<AMomentumCharacter>(GetCharacterOwner());
}

float UMomentumMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	
	if (MomentumCharacterOwner && MomentumCharacterOwner->bWantsToSprint)
	{
		return MaxSpeed * SprintSpeedMultiplier;
	}
	
	return MaxSpeed;
}

void UMomentumMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsWallRunning && CharacterOwner)
	{
		FVector WallRunDirection = FVector::ZeroVector;
		
		if (CurrentWallSide == EWallRunSide::Right)
		{
			WallRunDirection = FVector::CrossProduct(CurrentWallNormal, FVector::DownVector);
		} 
		else if (CurrentWallSide == EWallRunSide::Left)
		{
			WallRunDirection = FVector::CrossProduct(CurrentWallNormal, FVector::UpVector);
		}
		
		WallRunDirection.Normalize();
		
		FVector Start = CharacterOwner->GetActorLocation();
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(CharacterOwner);
		
		FHitResult ForwardHit;
		FVector ForwardEnd = Start + (WallRunDirection * 75.0f);
		
		if (GetWorld()->LineTraceSingleByChannel(ForwardHit, Start, ForwardEnd, ECC_Visibility, Params))
		{
			StopWallRun();
			return; 
		}
		
		FHitResult SideHit;
		bool bIsLeft = (CurrentWallSide == EWallRunSide::Left);
		
		if (!WallCheck(SideHit, bIsLeft))
		{
			StopWallRun();
			return;
		}
		else
		{
			CurrentWallNormal = SideHit.ImpactNormal;
		}

		FVector NewVelocity = WallRunDirection * MaxWallRunSpeed;
		
		NewVelocity += (-CurrentWallNormal * 150.f);

		Velocity = NewVelocity;
		
		FRotator TargetRotation = WallRunDirection.Rotation();
		FRotator CurrentRotation = CharacterOwner->GetActorRotation();
		
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 10.f);
		CharacterOwner->SetActorRotation(NewRotation);
	}
}

void UMomentumMovementComponent::TryWallRun()
{
	if (!IsFalling() || bIsWallRunning) return;
	
	FHitResult WallHit;
	
	if (WallCheck(WallHit, true))
	{
		StartWallRun(EWallRunSide::Left, WallHit.ImpactNormal);
		return;
	}

	if (WallCheck(WallHit, false))
	{
		StartWallRun(EWallRunSide::Right, WallHit.ImpactNormal);
	}
}

void UMomentumMovementComponent::DoWallJump()
{
	if (!CharacterOwner || !bIsWallRunning) return;
	
	FVector JumpNormal = CurrentWallNormal;
	
	StopWallRun();
	
	FVector JumpVelocity = (JumpNormal * WallJumpForce) + (FVector::UpVector * WallJumpVerticalForce) + 
		(CharacterOwner->GetActorForwardVector() * (WallJumpForce / 2.0f));
	
	CharacterOwner->LaunchCharacter(JumpVelocity, false, true);
}

bool UMomentumMovementComponent::WallCheck(FHitResult& WallHit, bool bLeft)
{
	if (!CharacterOwner) return false;
	
	FVector Start = CharacterOwner->GetActorLocation();
	
	FVector SideVector = bLeft ? -CharacterOwner->GetActorRightVector() : CharacterOwner->GetActorRightVector();
	
	FVector End = Start + (SideVector * WallCheckDistance);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CharacterOwner);
	
	return GetWorld()->LineTraceSingleByChannel(WallHit, Start, End, ECC_Visibility, Params);
}

void UMomentumMovementComponent::StartWallRun(EWallRunSide Side, const FVector& WallNormal)
{
	if (!CharacterOwner || bIsWallRunning) return;
	
	bIsWallRunning = true;
	CurrentWallSide = Side;
	CurrentWallNormal = WallNormal;
	
	SetMovementMode(MOVE_Flying);
	
	bOrientRotationToMovement = false;
	
	if (MomentumCharacterOwner)
	{
		MomentumCharacterOwner->AddStateTag(FMomentumGameplayTags::Get().State_Movement_WallRun);
	}
	
	GetWorld()->GetTimerManager().SetTimer(WallRunTimer, this, &UMomentumMovementComponent::StopWallRun, MaxWallRunTime, false);
}

void UMomentumMovementComponent::StopWallRun()
{
	if (!CharacterOwner || !bIsWallRunning) return;
	
	bIsWallRunning = false;
	CurrentWallSide = EWallRunSide::None;
	CurrentWallNormal = FVector::ZeroVector;
	
	GetWorld()->GetTimerManager().ClearTimer(WallRunTimer);
	
	SetMovementMode(MOVE_Falling);
	
	if (MomentumCharacterOwner)
	{
		MomentumCharacterOwner->RemoveStateTag(FMomentumGameplayTags::Get().State_Movement_WallRun);
	}
	
	bOrientRotationToMovement = true;
}
