// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MomentumPlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class PROJECTMOMENTUM_API AMomentumPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMomentumPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Input Mappings")
	TArray<TObjectPtr<UInputMappingContext>> DefaultMappingContexts;

private:
};
