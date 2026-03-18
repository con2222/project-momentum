#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "MomentumCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputComponent;
struct FInputActionValue;
class UMomentumMovementComponent;
class UCombatSystemComponent;

UCLASS()
class PROJECTMOMENTUM_API AMomentumCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMomentumCharacter(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveStateTags;
	
	void AddStateTag(const FGameplayTag& TagToAssign);
	void RemoveStateTag(const FGameplayTag& TagToRemove);
	bool HasStateTag(const FGameplayTag& TagToCheck) const;
	
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
	
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UMomentumMovementComponent> MomentumMovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UCombatSystemComponent> CombatSystemComponent;
	
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;
	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
};
