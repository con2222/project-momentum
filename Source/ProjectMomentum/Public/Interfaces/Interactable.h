

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTMOMENTUM_API IInteractable
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void Interact(AActor* Interactor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void LookAt();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void StopLookingAt();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void Dissolve();
};
