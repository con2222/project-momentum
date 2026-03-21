

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "GameplayTagContainer.h"
#include "Data/WeaponData.h"
#include "MomentumBaseWeapon.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;

UCLASS()
class PROJECTMOMENTUM_API AMomentumBaseWeapon : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AMomentumBaseWeapon();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UBoxComponent> Box;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachSocketName = "WeaponSocket";
public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponData WeaponData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeToDestroyAfterEquip = 3.f;
	
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual void LookAt_Implementation() override;
	virtual void StopLookingAt_Implementation() override;
	
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void Dissolve_Implementation() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactable")
	void StartDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic, bool bSpawn);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	void EquipWeapon(AActor* NewOwner);
	void PlayEquipMontage(AActor* NewOwner);
	
	void SpawnWeapon();
};
