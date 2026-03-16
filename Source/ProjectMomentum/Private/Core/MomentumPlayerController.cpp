// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MomentumPlayerController.h"

#include "EnhancedInputSubsystems.h"

AMomentumPlayerController::AMomentumPlayerController()
{
}

void AMomentumPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AMomentumPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		for (const auto Context : DefaultMappingContexts)
		{
			check(Context);
			Subsystem->AddMappingContext(Context, 0);
		}
	}
}

void AMomentumPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}