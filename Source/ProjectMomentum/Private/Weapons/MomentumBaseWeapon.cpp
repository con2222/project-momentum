 


#include "Weapons/MomentumBaseWeapon.h"

AMomentumBaseWeapon::AMomentumBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	
	RootComponent = WeaponMesh;
	
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMomentumBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMomentumBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

