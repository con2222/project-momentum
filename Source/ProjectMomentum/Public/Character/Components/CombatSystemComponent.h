
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/WeaponData.h"
#include "CombatSystemComponent.generated.h"

class UChooserTable;
class AMomentumCharacter;
class UMomentumAnimInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTMOMENTUM_API UCombatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatSystemComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animations")
	TObjectPtr<UChooserTable> UnarmedAttacksChooserTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> EnterStanceMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> ExitStanceMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animation")
	TSubclassOf<UAnimInstance> StanceAnimLayer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
	int32 ComboCount = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FWeaponData> WeaponsInventory;
	
	void Attack();
	bool CanAttack() const;
	
	UFUNCTION(BlueprintCallable)
	void AddCombo();
	
	UFUNCTION(BlueprintCallable)
	void ResetCombo();
	
	UFUNCTION(BlueprintCallable)
	void EquipWeaponFromInventory(int InventoryIndex);
	
	UFUNCTION(BlueprintCallable)
	void UnEquipCurrentWeapon();
	
	UFUNCTION(BlueprintCallable)
	void RemoveEquippedTag();
	
	UFUNCTION(BlueprintCallable)
	void SetUnArmedLayer();

protected:
	virtual void BeginPlay() override;
	
	FTimerHandle CombatTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat|State")
	float TimeToExitCombat = 5.f;
	
	void ExitCombatMode();
	
	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<AMomentumBaseWeapon> EquippedWeapon;
	
	UPROPERTY()
	TObjectPtr<AMomentumCharacter> MomentumCharacterOwner;
	
	UPROPERTY()
	TObjectPtr<UMomentumAnimInstance> AnimInstance;

private:
	UPROPERTY()
	UAnimMontage* AttackMontage;
	
public:
	FORCEINLINE AMomentumBaseWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
	
	void SetEquippedWeapon(AMomentumBaseWeapon* Weapon) { EquippedWeapon = Weapon; }
};
