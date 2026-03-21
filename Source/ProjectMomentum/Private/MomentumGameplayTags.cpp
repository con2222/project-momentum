


#include "MomentumGameplayTags.h"

#include "GameplayTagsManager.h"

FMomentumGameplayTags FMomentumGameplayTags::GameplayTags;

void FMomentumGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
	
	GameplayTags.State_Movement_WallRun = GameplayTagsManager.AddNativeGameplayTag(
		FName("State.Movement.WallRun"),
		FString("Character is currently wall running")	
	);
	
	GameplayTags.State_Combat_Attacking = GameplayTagsManager.AddNativeGameplayTag(
		FName("State.Combat.Attacking"),
		FString("Character is currently attacking")
	);
	
	GameplayTags.State_Action_Interact = GameplayTagsManager.AddNativeGameplayTag(
		FName("State.Action.Interact"),
		FString("Character is currently Interact")
	);
	
	GameplayTags.State_Action_EquipWeapon = GameplayTagsManager.AddNativeGameplayTag(
		FName("State.Action.EquipWeapon"),
		FString("EquipWeapon")
	);
	
	GameplayTags.State_Movement_Grounded = GameplayTagsManager.AddNativeGameplayTag(
		FName("State.Movement.Grounded"),
		FString("Character is currently on Grounded")
	);
	
	GameplayTags.State_Movement_Airborne = GameplayTagsManager.AddNativeGameplayTag(
		FName("State.Movement.Airborne"),
		FString("Character is currently in Airborne")
	);
	
	GameplayTags.Weapon_Katana = GameplayTagsManager.AddNativeGameplayTag(
		FName("Weapon.Katana"),
		FString("Katana's tag")
	);
}