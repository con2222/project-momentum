
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct PROJECTMOMENTUM_API FMomentumGameplayTags
{
public:
	static const FMomentumGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	
	FGameplayTag State_Movement_WallRun;
	FGameplayTag State_Movement_Grounded;
	FGameplayTag State_Movement_Airborne;
	
	FGameplayTag State_Action_Interact;
	FGameplayTag State_Action_EquipWeapon;
	
	FGameplayTag State_Combat_Active;
	FGameplayTag State_Combat_Attacking;
	
	FGameplayTag Weapon_Katana;
private:
	static FMomentumGameplayTags GameplayTags;	
};