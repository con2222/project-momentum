
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatSystemComponent.generated.h"

class UChooserTable;
class AMomentumCharacter;
class UMomentumAnimInstance;
class AMomentumBaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTMOMENTUM_API UCombatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatSystemComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animations")
	TObjectPtr<UChooserTable> AttacksChooserTable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
	int32 ComboCount = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Equipment")
	TSubclassOf<AMomentumBaseWeapon> DefaultWeaponClass;
	
	void Attack();
	bool CanAttack() const;
	
	UFUNCTION(BlueprintCallable)
	void AddCombo();
	
	UFUNCTION(BlueprintCallable)
	void ResetCombo();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<AMomentumBaseWeapon> EquippedWeapon;
	
	UPROPERTY()
	TObjectPtr<AMomentumCharacter> MomentumCharacterOwner;
	
	UPROPERTY()
	TObjectPtr<UMomentumAnimInstance> AnimInstance;

private:
	UPROPERTY()
	UAnimMontage* AttackMontage;
};
