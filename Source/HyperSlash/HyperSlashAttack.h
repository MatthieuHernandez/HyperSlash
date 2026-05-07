// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HyperSlashAttack.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS(abstract)
class AHyperSlashAttack : public AActor
{
	GENERATED_BODY()

	/** Provides the visual representation for the attack */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SphereVisual;

	/** Provides the collision volume for the attack */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;

protected:

	/** Timer to start AoE damage checks */
	FTimerHandle StartAoETimer;

	/** Timer to end AoE damage checks */
	FTimerHandle StopAoETimer;

	/** Time to wait between AoE damage ticks */
	UPROPERTY(EditAnywhere, Category = "Attack", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float StartAoETime = 0.033f;

	/** Time to wait before stopping AoE damage checks */
	UPROPERTY(EditAnywhere, Category = "Attack", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float StopAoETime = 0.5f;

	/** While true, the AoE will damage anything that overlaps it */
	bool bIsAoEActive = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Cleanup */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Called when the start Attack timer triggers */
	void StartAttack();

	/** Called when the stop Attack timer triggers */
	void StopAttack();

	/** Allows Blueprint handling of AoE fade out effects. NOTE: Call Destroy Actor at the end of this! */
	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	void BP_AoEFinished();

	/** Handles collision with the AoE while it's active */
	UFUNCTION()
	void OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:

	// Sets default values for this actor's properties
	AHyperSlashAttack();
};
