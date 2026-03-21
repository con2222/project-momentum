
#include "Character/MomentumCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "MomentumGameplayTags.h"
#include "Character/Components/CombatSystemComponent.h"
#include "Character/Components/MomentumMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/Interactable.h"
#include "Weapons/MomentumBaseWeapon.h"


AMomentumCharacter::AMomentumCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMomentumMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	CombatSystemComponent = CreateDefaultSubobject<UCombatSystemComponent>(FName("CombatSystemComponent"));
	
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

void AMomentumCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	EMovementMode CurrentMode = GetCharacterMovement()->MovementMode;
	
	if (CurrentMode == MOVE_Walking || CurrentMode == MOVE_NavWalking)
	{
		AddStateTag(FMomentumGameplayTags::Get().State_Movement_Grounded);
		RemoveStateTag(FMomentumGameplayTags::Get().State_Movement_Airborne);
	}  else if (CurrentMode == MOVE_Falling)
	{
		AddStateTag(FMomentumGameplayTags::Get().State_Movement_Airborne);
		RemoveStateTag(FMomentumGameplayTags::Get().State_Movement_Grounded);
	}
}

void AMomentumCharacter::AddStateTag(const FGameplayTag& TagToAssign)
{
	CharacterTags.AddTag(TagToAssign);
}

void AMomentumCharacter::RemoveStateTag(const FGameplayTag& TagToRemove)
{
	CharacterTags.RemoveTag(TagToRemove);
}

bool AMomentumCharacter::HasStateTag(const FGameplayTag& TagToCheck) const
{
	return CharacterTags.HasTag(TagToCheck);
}

bool AMomentumCharacter::HasStateTagExact(const FGameplayTag& TagToCheck) const
{
	return CharacterTags.HasTagExact(TagToCheck);
}

void AMomentumCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterTags.AddTag(FMomentumGameplayTags::Get().State_Movement_Grounded);
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && UnarmedAnimLayer)
	{
		AnimInstance->LinkAnimClassLayers(UnarmedAnimLayer);
	}
}

void AMomentumCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	MomentumMovementComponent = Cast<UMomentumMovementComponent>(GetCharacterMovement());
}

void AMomentumCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float TargetRoll = 0.f;
	if (MomentumMovementComponent && MomentumMovementComponent->IsWallRunning())
	{
		if (MomentumMovementComponent->GetCurrentWallSide() == EWallRunSide::Left)
		{
			TargetRoll = -MaxCameraTilt;
		} 
		else if (MomentumMovementComponent->GetCurrentWallSide() == EWallRunSide::Right)
		{
			TargetRoll = MaxCameraTilt;
		}
	}
	
	FRotator CurrentCameraRotation = FollowCamera->GetRelativeRotation();
	float NewRoll = FMath::FInterpTo(CurrentCameraRotation.Roll, TargetRoll, DeltaTime, CameraTiltSpeed);
	FollowCamera->SetRelativeRotation(FRotator(CurrentCameraRotation.Pitch, CurrentCameraRotation.Yaw, NewRoll));
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	FHitResult Hit;
	FVector TraceStart = FollowCamera->GetComponentLocation();
	FVector TraceEnd = TraceStart + (FollowCamera->GetForwardVector() * SphereTraceRange);
	
	UKismetSystemLibrary::SphereTraceSingle(this, TraceStart, 
		TraceEnd, 20, TraceTypeQuery3, false, ActorsToIgnore, 
		EDrawDebugTrace::ForOneFrame, Hit, true, FLinearColor::Blue, FLinearColor::Red, 20.f);
	
	AActor* HitActor = Hit.GetActor();
	if (HitActor && HitActor->Implements<UInteractable>())
	{
		float DistanceToItem = FVector::Dist(GetActorLocation(), HitActor->GetActorLocation());
		
		if (DistanceToItem <= InteractionDistance)
		{
			if (HitActor != CurrentInteractable)
			{
				if (CurrentInteractable)
				{
					IInteractable::Execute_StopLookingAt(CurrentInteractable);
				}
				
				IInteractable::Execute_LookAt(HitActor);
				CurrentInteractable = HitActor;
			}
		} else
		{
			if (CurrentInteractable)
			{
				IInteractable::Execute_StopLookingAt(CurrentInteractable);
				CurrentInteractable = nullptr;
			}
		}
		
		IInteractable::Execute_LookAt(HitActor);
		CurrentInteractable = HitActor;
	} else
	{
		if (CurrentInteractable)
		{
			IInteractable::Execute_StopLookingAt(CurrentInteractable);
			CurrentInteractable = nullptr;
		}
	}
	
	if (CurrentInteractable)
	{
		FString DebugMsg = FString::Printf(TEXT("Interact %s"), *CurrentInteractable->GetActorNameOrLabel());
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Yellow, DebugMsg);
	} else
	{
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Yellow, TEXT("Not Interact"));
	}
}

void AMomentumCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMomentumCharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMomentumCharacter::StopJumping);
		}
		
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMomentumCharacter::Move);
		}
		
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMomentumCharacter::Look);
		}
		
		if (RunAction)
		{
			EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AMomentumCharacter::SprintStarted);
			EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMomentumCharacter::SprintCompleted);
		}
		if (AttackAction)
		{
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, CombatSystemComponent.Get(), &UCombatSystemComponent::Attack);
		}
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMomentumCharacter::Interact);
		}
		if (EquipAction)
		{
			EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &AMomentumCharacter::EquipWeapon);
		}
	}
}

void AMomentumCharacter::Move(const FInputActionValue& Value)
{
	if (MomentumMovementComponent && MomentumMovementComponent->IsWallRunning())
	{
		return;
	}
	
	if (HasStateTag(FGameplayTag::RequestGameplayTag(FName("State.Action")))) return;
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (!MovementVector.IsNearlyZero())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && AnimInstance->Montage_IsPlaying(PickupMontage))
		{
			AnimInstance->Montage_Stop(0.25f, PickupMontage);
		}
	}
	
	if (Controller)
	{
		FRotator ControllRotation = GetControlRotation();
		FRotator YawRotation(0.f, ControllRotation.Yaw, 0.f);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMomentumCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if (Controller)
	{
		AddControllerPitchInput(LookAxisVector.Y);
		AddControllerYawInput(LookAxisVector.X);
	}
}

void AMomentumCharacter::Jump()
{
	if (HasStateTag(FGameplayTag::RequestGameplayTag(FName("State.Action")))) return;
	
	Super::Jump();
	
	if (MomentumMovementComponent && MomentumMovementComponent->IsWallRunning())
	{
		MomentumMovementComponent->DoWallJump();
	}
	else
	{
		Super::Jump();
	}
}

void AMomentumCharacter::StopJumping()
{
	Super::StopJumping();
}

void AMomentumCharacter::SprintStarted()
{
	if (HasStateTag(FGameplayTag::RequestGameplayTag(FName("State.Action")))) return;
	bWantsToSprint = true;
	
	if (MomentumMovementComponent)
	{
		if (MomentumMovementComponent->IsFalling())
		{
			MomentumMovementComponent->TryWallRun();
		}
	}
}

void AMomentumCharacter::SprintCompleted()
{
	if (HasStateTag(FGameplayTag::RequestGameplayTag(FName("State.Action")))) return;
	bWantsToSprint = false;
	
	if (MomentumMovementComponent)
	{
		MomentumMovementComponent->StopWallRun();
	}
}

void AMomentumCharacter::Interact()
{
	if (!CurrentInteractable || HasStateTag(FGameplayTag::RequestGameplayTag(FName("State.Action")))) return;
	
	float DistanceToItem = FVector::Dist(GetActorLocation(), CurrentInteractable->GetActorLocation());
	if (CurrentInteractable && DistanceToItem < InteractionDistance)
	{
		CharacterTags.AddTag(FMomentumGameplayTags::Get().State_Action_Interact);
		InteractableItem = CurrentInteractable;
		IInteractable::Execute_Interact(InteractableItem, this);
	}
}

void AMomentumCharacter::EquipWeapon()
{
	if (HasStateTag(FGameplayTag::RequestGameplayTag(FName("State.Action"))) || !HasStateTag(FMomentumGameplayTags::Get().State_Movement_Grounded) || CombatSystemComponent->WeaponsInventory.Num() == 0) return;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();	
	if (CombatSystemComponent->WeaponsInventory.Num() > 0)
	{
		FWeaponData EquippedWeaponData = CombatSystemComponent->WeaponsInventory[0];
		
		if (!HasStateTag(FGameplayTag::RequestGameplayTag(FName("Weapon"))))
		{
			if (AnimInstance && EquippedWeaponData.EquipWeaponMontage)
			{
				AnimInstance->Montage_Play(EquippedWeaponData.EquipWeaponMontage);
				AddStateTag(FMomentumGameplayTags::Get().State_Action_EquipWeapon);
				AddStateTag(FMomentumGameplayTags::Get().State_Combat_Active);
			}
		} else
		{
			if (AnimInstance && EquippedWeaponData.UnEquipWeaponMontage)
			{
				AnimInstance->Montage_Play(EquippedWeaponData.UnEquipWeaponMontage);
				AddStateTag(FMomentumGameplayTags::Get().State_Action_EquipWeapon);
				RemoveStateTag(FMomentumGameplayTags::Get().State_Combat_Active);
			}
		}
	}
}

void AMomentumCharacter::PrintActiveTags()
{
	FString TagsString = CharacterTags.ToStringSimple();
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Cyan, FString::Printf(TEXT("Tags: %s"), *TagsString));
	}
}

void AMomentumCharacter::ExecuteInteraction()
{
	if (InteractableItem)
	{
		RemoveStateTag(FMomentumGameplayTags::Get().State_Action_Interact);
		CurrentInteractable = nullptr; 
		InteractableItem = nullptr;
	}
}
