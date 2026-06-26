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
    hitbox = Cast<UBoxComponent>(GetDefaultSubobjectByName(TEXT("Hitbox")));
    if (hitbox)
    {
        hitbox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnHit);
        DisableHitbox();
    }
}

void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWeapon::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AHyperSlashEnemy* Enemy = Cast<AHyperSlashEnemy>(OtherActor))
    {
        Enemy->ProjectileImpact(FVector::ZeroVector);
    }
}

void AWeapon::EnableHitbox()
{
    if (hitbox)
    {
        hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        hitbox->SetGenerateOverlapEvents(true);
    }
}

void AWeapon::DisableHitbox()
{
    if (hitbox)
    {
        hitbox->SetGenerateOverlapEvents(false);
        hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}