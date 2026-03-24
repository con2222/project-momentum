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
	
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer CharacterTags;
	
	UFUNCTION(BlueprintCallable, Category = "Tags")
	void AddStateTag(const FGameplayTag& TagToAssign);
	
	UFUNCTION(BlueprintCallable, Category = "Tags")
	void RemoveStateTag(const FGameplayTag& TagToRemove);
	
	UFUNCTION(BlueprintCallable, Category = "Tags")
	bool HasStateTag(const FGameplayTag& TagToCheck) const;
	
	UFUNCTION(BlueprintCallable, Category = "Tags")
	bool HasStateTagExact(const FGameplayTag& TagToCheck) const;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bWantsToSprint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation Layers")
	TSubclassOf<UAnimInstance> UnarmedAnimLayer;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/* Callbacks for Input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	virtual void StopJumping() override;
	void SprintStarted();
	void SprintCompleted();
	void Interact();
	void EquipWeapon();
	/* Callbacks for Input */
	
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void PrintActiveTags();

	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ExecuteInteraction();
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Animation")
	TObjectPtr<UAnimMontage> PickupMontage;
	
	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float SphereTraceRange = 1500.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> CurrentInteractable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> InteractableItem;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionDistance = 200.f;
	
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> EquipAction;
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
public:
	FORCEINLINE UCombatSystemComponent* GetCombatSystemComponent() const { return CombatSystemComponent; }
	FORCEINLINE UAnimMontage* GetPickupMontage() const { return PickupMontage; }
};
