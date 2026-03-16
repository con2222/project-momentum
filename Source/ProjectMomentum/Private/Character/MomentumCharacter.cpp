
#include "Character/MomentumCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "Character/Components/MomentumMovementComponent.h"


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
	
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

void AMomentumCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMomentumCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	}
}

void AMomentumCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
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
	Super::Jump();
}

void AMomentumCharacter::StopJumping()
{
	Super::StopJumping();
}

void AMomentumCharacter::SprintStarted()
{
	bWantsToSprint = true;
}

void AMomentumCharacter::SprintCompleted()
{
	bWantsToSprint = false;
}

