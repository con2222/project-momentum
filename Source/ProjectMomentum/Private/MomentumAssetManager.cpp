


#include "MomentumAssetManager.h"

#include "MomentumGameplayTags.h"

UMomentumAssetManager& UMomentumAssetManager::Get()
{
	check(GEngine);
	UMomentumAssetManager* MomentumAssetManager = Cast<UMomentumAssetManager>(GEngine->AssetManager);
	return *MomentumAssetManager;
}

void UMomentumAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FMomentumGameplayTags::InitializeNativeGameplayTags();
}
