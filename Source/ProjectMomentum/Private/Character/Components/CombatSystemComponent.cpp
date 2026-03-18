
#include "Character/Components/CombatSystemComponent.h"
#include "Character/MomentumCharacter.h"
#include "MomentumGameplayTags.h"
#include "ChooserFunctionLibrary.h"
#include "Character/Animation/MomentumAnimInstance.h"


UCombatSystemComponent::UCombatSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UCombatSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	MomentumCharacterOwner = Cast<AMomentumCharacter>(GetOwner());
	if (MomentumCharacterOwner)
	{
		AnimInstance = Cast<UMomentumAnimInstance>(MomentumCharacterOwner->GetMesh()->GetAnimInstance());
	}
}


void UCombatSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UCombatSystemComponent::CanAttack() const
{
	if (!MomentumCharacterOwner) return false;
	
	if (MomentumCharacterOwner->HasStateTag(FMomentumGameplayTags::Get().State_Movement_WallRun))
	{
		return false;
	}
	
	if (MomentumCharacterOwner->HasStateTag(FMomentumGameplayTags::Get().State_Combat_Attacking))
	{
		return false;
	}
	return true;
}

void UCombatSystemComponent::Attack()
{
	if (!CanAttack() || !AttacksChooserTable || !AnimInstance) return;
	
	UAnimMontage* MontageToPlay = Cast<UAnimMontage>(UChooserFunctionLibrary::EvaluateChooser(this, 
		AttacksChooserTable, UAnimMontage::StaticClass()));
	if (MontageToPlay)
	{
		MomentumCharacterOwner->AddStateTag(FMomentumGameplayTags::Get().State_Combat_Attacking);
		
		AnimInstance->Montage_Play(MontageToPlay);
	}
}

void UCombatSystemComponent::AddCombo()
{
	ComboCount++;
	
	if (MomentumCharacterOwner)
	{
		MomentumCharacterOwner->RemoveStateTag(FMomentumGameplayTags::Get().State_Combat_Attacking);
	}
}

void UCombatSystemComponent::ResetCombo()
{
	ComboCount = 0;
	
	if (MomentumCharacterOwner)
	{
		MomentumCharacterOwner->RemoveStateTag(FMomentumGameplayTags::Get().State_Combat_Attacking);
	}
}