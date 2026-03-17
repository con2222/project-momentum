
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct PROJECTMOMENTUM_API FMomentumGameplayTags
{
public:
	static const FMomentumGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	
	FGameplayTag State_Movement_WallRun;
	FGameplayTag State_Combat_Attacking;
	
private:
	static FMomentumGameplayTags GameplayTags;	
};