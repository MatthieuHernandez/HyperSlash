#include "Weapon.h"
#include "HyperSlashCharacter.h"
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
    if (!OtherActor || OtherActor == GetOwner()) return; // To avoid hurting itself
    if (auto* enemy = Cast<AHyperSlashEnemy>(OtherActor))
    {
        if (auto* player = Cast<AHyperSlashCharacter>(GetOwner()))
        {
            auto knockback = (enemy->GetActorLocation() - player->GetActorLocation()).GetSafeNormal();
            knockback.Z = 0.0f;
            enemy->GetHit(knockback);
        }
    }
    else if (auto* player = Cast<AHyperSlashCharacter>(OtherActor))
    {
        if (Cast<AHyperSlashEnemy>(GetOwner()))
        {
            player->BeHit(Direction::Front);
        }
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