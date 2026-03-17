


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
}