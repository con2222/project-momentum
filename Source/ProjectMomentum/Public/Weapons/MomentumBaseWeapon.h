

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MomentumBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class PROJECTMOMENTUM_API AMomentumBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AMomentumBaseWeapon();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

public:	
	virtual void Tick(float DeltaTime) override;

};
