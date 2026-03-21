

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */

struct FGameplayTag;

USTRUCT(BlueprintType, Blueprintable)
struct FWeaponInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WeaponTag = FGameplayTag();
	
};

UCLASS()
class PROJECTMOMENTUM_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	TMap<FGameplayTag, FWeaponInfo> WeaponsInfo;
};
