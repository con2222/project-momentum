#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WeaponData.generated.h"

class AMomentumBaseWeapon;
class UTexture2D;
class UChooserTable;

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data")
	FGameplayTag WeaponTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Data")
	FText WeaponName = FText::FromString("Empty");
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Data")
	TObjectPtr<UTexture2D> WeaponIcon = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Data")
	float BaseDamage = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Data")
	TSubclassOf<AMomentumBaseWeapon> WeaponClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animations")
	TObjectPtr<UChooserTable> AttacksChooserTable = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	TSubclassOf<UAnimInstance> WeaponAnimLayer = UAnimInstance::StaticClass();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	TObjectPtr<UAnimMontage> EquipWeaponMontage =  nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	TObjectPtr<UAnimMontage> UnEquipWeaponMontage = nullptr;
};
