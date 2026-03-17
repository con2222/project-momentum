

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MomentumAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOMENTUM_API UMomentumAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UMomentumAssetManager & Get();
protected:
	virtual void StartInitialLoading() override;
};
