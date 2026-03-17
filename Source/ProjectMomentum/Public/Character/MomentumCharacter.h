#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MomentumCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputComponent;
struct FInputActionValue;

UCLASS()
class PROJECTMOMENTUM_API AMomentumCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMomentumCharacter(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bWantsToSprint;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/* Callbacks for Input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	virtual void StopJumping() override;
	void SprintStarted();
	void SprintCompleted();
	/* Callbacks for Input */

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera|WallRun")
	float MaxCameraTilt = 15.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera|WallRun")
	float CameraTiltSpeed = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> RunAction;
	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
};
