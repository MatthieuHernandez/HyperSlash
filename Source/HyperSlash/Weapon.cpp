#include "Weapon.h"
#include "HyperSlashEnemy.h"
#include "Components/BoxComponent.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();
    Hitbox = Cast<UBoxComponent>(GetDefaultSubobjectByName(TEXT("Hitbox")));
    if (Hitbox)
    {
        Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnHit);
    }
}

void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWeapon::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("AWeapon::OnHit called"));
    if (AHyperSlashEnemy* Enemy = Cast<AHyperSlashEnemy>(OtherActor))
    {
        Enemy->ProjectileImpact(FVector::ZeroVector);
    }
}
