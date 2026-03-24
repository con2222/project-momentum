
#include "Character/Player/Components/CombatSystemComponent.h"
#include "Character/Player/MomentumCharacter.h"
#include "MomentumGameplayTags.h"
#include "ChooserFunctionLibrary.h"
#include "Character/Player/Animation/MomentumAnimInstance.h"
#include "Weapons/MomentumBaseWeapon.h"


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

void UCombatSystemComponent::ExitCombatMode()
{
	if (MomentumCharacterOwner)
	{
		UAnimInstance* MomentumAnimInstance = MomentumCharacterOwner->GetMesh()->GetAnimInstance();
		if (MomentumAnimInstance)
		{
			MomentumAnimInstance->LinkAnimClassLayers(MomentumCharacterOwner->UnarmedAnimLayer);
		}
		ResetCombo();
	}
	GetWorld()->GetTimerManager().ClearTimer(CombatTimerHandle);
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
	
	if (MomentumCharacterOwner->HasStateTag(FMomentumGameplayTags::Get().State_Movement_Airborne))
	{
		return false;
	}
	if (MomentumCharacterOwner->HasStateTag(FMomentumGameplayTags::Get().State_Movement_WallRun))
	{
		return false;
	}
	
	return true;
}

void UCombatSystemComponent::Attack()
{
	if (!CanAttack() || !UnarmedAttacksChooserTable || !AnimInstance || MomentumCharacterOwner->HasStateTag(FGameplayTag::RequestGameplayTag(FName("State.Action")))) return;
	
	if (EquippedWeapon)
	{
		UAnimMontage* MontageToPlay = Cast<UAnimMontage>(UChooserFunctionLibrary::EvaluateChooser(this, 
		EquippedWeapon->WeaponData.AttacksChooserTable, UAnimMontage::StaticClass()));
		if (MontageToPlay)
		{
			MomentumCharacterOwner->AddStateTag(FMomentumGameplayTags::Get().State_Combat_Attacking);
		
			AnimInstance->Montage_Play(MontageToPlay);
		}
	} else
	{
		MomentumCharacterOwner->AddStateTag(FMomentumGameplayTags::Get().State_Combat_Active);
		GetWorld()->GetTimerManager().SetTimer(CombatTimerHandle, this, &UCombatSystemComponent::ExitCombatMode, TimeToExitCombat, false);
		
		UAnimInstance* MomentumAnimInstance = MomentumCharacterOwner->GetMesh()->GetAnimInstance();
		if (MomentumAnimInstance)
		{
			MomentumAnimInstance->LinkAnimClassLayers(StanceAnimLayer);
		}
		
		UAnimMontage* MontageToPlay = Cast<UAnimMontage>(UChooserFunctionLibrary::EvaluateChooser(this, 
		UnarmedAttacksChooserTable, UAnimMontage::StaticClass()));
		if (MontageToPlay)
		{
			MomentumCharacterOwner->AddStateTag(FMomentumGameplayTags::Get().State_Combat_Attacking);
			AnimInstance->Montage_Play(MontageToPlay);
		}
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

void UCombatSystemComponent::EquipWeaponFromInventory(int InventoryIndex = 0)
{
	if (!WeaponsInventory.IsValidIndex(InventoryIndex)) return;
	
	FWeaponData WeaponData = WeaponsInventory[InventoryIndex];
	
	if (!WeaponData.WeaponClass) 
	{
		UE_LOG(LogTemp, Error, TEXT("Not have TSubClassOf in WeaponData"));
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform SpawnTransform = GetOwner()->GetActorTransform();
	
	AMomentumBaseWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AMomentumBaseWeapon>(
		WeaponData.WeaponClass, 
		SpawnTransform, 
		SpawnParams
	);
	
	
	
	if (SpawnedWeapon)
	{
		SpawnedWeapon->WeaponData = WeaponData;
		SpawnedWeapon->EquipWeapon(MomentumCharacterOwner);
		SpawnedWeapon->SpawnWeapon();
		SetEquippedWeapon(SpawnedWeapon);
		
		if (MomentumCharacterOwner && MomentumCharacterOwner->GetMesh())
		{
			MomentumCharacterOwner->AddStateTag(WeaponData.WeaponTag);
			UAnimInstance* MomentumAnimInstance = MomentumCharacterOwner->GetMesh()->GetAnimInstance();
           
			if (MomentumAnimInstance)
			{
				if (WeaponData.WeaponAnimLayer)
				{
					MomentumAnimInstance->LinkAnimClassLayers(WeaponData.WeaponAnimLayer);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("In weapon %s not chosen WeaponAnimLayer!"), *WeaponData.WeaponClass->GetName());
				}
			}
		}
	}
}

void UCombatSystemComponent::UnEquipCurrentWeapon()
{
	if (EquippedWeapon)
	{
		MomentumCharacterOwner->RemoveStateTag(EquippedWeapon->WeaponData.WeaponTag); 
		EquippedWeapon->Dissolve_Implementation();
		EquippedWeapon = nullptr;
	}
}

void UCombatSystemComponent::RemoveEquippedTag()
{
	MomentumCharacterOwner->RemoveStateTag(FMomentumGameplayTags::Get().State_Action_EquipWeapon);
}

void UCombatSystemComponent::SetUnArmedLayer()
{
	UAnimInstance* MomentumAnimInstance = MomentumCharacterOwner->GetMesh()->GetAnimInstance();
	if (MomentumAnimInstance)
	{
		MomentumAnimInstance->LinkAnimClassLayers(MomentumCharacterOwner->UnarmedAnimLayer);
	}
}
