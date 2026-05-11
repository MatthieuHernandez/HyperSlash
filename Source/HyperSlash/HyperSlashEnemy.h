#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HyperSlashEnemy.generated.h"

class ATwinStickNPCDestruction;

UCLASS(abstract)
class AHyperSlashEnemy : public ACharacter
{
	GENERATED_BODY()

protected:
	/** Type of destruction proxy to spawn on death */
	UPROPERTY(EditAnywhere, Category = "Destruction")
	TSubclassOf<ATwinStickNPCDestruction> DestructionProxyClass;

	/** Time to wait after this NPC is hit before destroying it */
	UPROPERTY(EditAnywhere, Category = "Destruction", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float DeferredDestructionTime = 0.1f;

	/** Deferred destruction timer */
	FTimerHandle DestructionTimer;

	/** Gameplay Initialization */
	virtual void BeginPlay() override;

	/** Gameplay cleanup */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Handle destruction */
	virtual void Destroyed() override;

	/** Collision handling */
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	void StopStateTreeLogic();

	/** Called from timer to complete the destruction process for this NPC */
	void DeferredDestroy();

public:	

	/** If true, this NPC has already been hit by a projectile and is being destroyed. Exposed to BP so it can be read by StateTree */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	bool bHit = false;

	// Sets default values for this character's properties
	AHyperSlashEnemy();

	/** Tells the NPC to process a projectile impact */
	void ProjectileImpact(const FVector& ForwardVector);
};
