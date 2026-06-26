#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Weapon.generated.h"

//class UBoxComponent;

UCLASS(abstract)
class HYPERSLASH_API AWeapon : public AActor
{
    GENERATED_BODY()

private:
    UBoxComponent* hitbox;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    AWeapon();

    virtual void Tick(float DeltaTime) override;

    void EnableHitbox();

    void DisableHitbox();
};
