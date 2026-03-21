 


#include "Weapons/MomentumBaseWeapon.h"
#include "Character/MomentumCharacter.h"
#include "Character/Components/CombatSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AMomentumBaseWeapon::AMomentumBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	
	RootComponent = WeaponMesh;
	
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}

void AMomentumBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (WeaponMesh)
	{
		DynamicMaterial = WeaponMesh->CreateAndSetMaterialInstanceDynamic(0);
	}
	
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(FName("GlowIntensity"), 0.f);
	}
	
}

void AMomentumBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMomentumBaseWeapon::Interact_Implementation(AActor* Interactor)
{
	if (!Interactor) return;
	
	AMomentumCharacter* CharacterOwner = Cast<AMomentumCharacter>(Interactor);
	SetOwner(Interactor);
	CharacterOwner->GetCombatSystemComponent()->WeaponsInventory.Add(WeaponData);
	PlayEquipMontage(Interactor);
}

void AMomentumBaseWeapon::LookAt_Implementation()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(FName("GlowIntensity"), 100.f);
	}
}

void AMomentumBaseWeapon::StopLookingAt_Implementation()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(FName("GlowIntensity"), 0.f);
	}
}

void AMomentumBaseWeapon::EquipWeapon(AActor* NewOwner)
{
	AMomentumCharacter* CharacterOwner = Cast<AMomentumCharacter>(NewOwner);
	if (CharacterOwner)
	{
		WeaponMesh->AttachToComponent(
			CharacterOwner->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			AttachSocketName
		);
		CharacterOwner->GetCombatSystemComponent()->SetEquippedWeapon(this);
		Box->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}
}

void AMomentumBaseWeapon::PlayEquipMontage(AActor* NewOwner)
{
	AMomentumCharacter* CharacterOwner = Cast<AMomentumCharacter>(NewOwner);
	UAnimMontage* PickupMontage = CharacterOwner->GetPickupMontage();
	
	if (PickupMontage)
	{
		CharacterOwner->PlayAnimMontage(PickupMontage);
	} else
	{
		CharacterOwner->ExecuteInteraction();
	}
}

void AMomentumBaseWeapon::SpawnWeapon()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicDissolveMaterial = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		WeaponMesh->SetMaterial(0, DynamicDissolveMaterial);
		StartDissolveTimeline(DynamicDissolveMaterial, true);
	}
}

void AMomentumBaseWeapon::Dissolve_Implementation()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicDissolveMaterial = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		WeaponMesh->SetMaterial(0, DynamicDissolveMaterial);
		StartDissolveTimeline(DynamicDissolveMaterial, false);
		SetLifeSpan(TimeToDestroyAfterEquip);
	}
}