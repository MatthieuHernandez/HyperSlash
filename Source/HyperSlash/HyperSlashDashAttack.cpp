#include "HyperSlashDashAttack.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HyperSlashEnemy.h"

// Sets default values
AHyperSlashDashAttack::AHyperSlashDashAttack()
{
	PrimaryActorTick.bCanEverTick = true;

	// create the root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// create the mesh that provides the visual representation for the AoE
	CapsuleVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Capsule Visual"));
	CapsuleVisual->SetupAttachment(RootComponent);

	CapsuleVisual->SetCollisionProfileName(FName("NoCollision"));

	// create the collision capsule
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule"));
	CollisionCapsule->SetupAttachment(RootComponent);


	CollisionCapsule->SetNotifyRigidBodyCollision(true);
	CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionCapsule->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AHyperSlashDashAttack::OnAttackOverlap);
}

// Called when the game starts or when spawned
void AHyperSlashDashAttack::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(StartAoETimer, this, &AHyperSlashDashAttack::StartAttack, StartAoETime, false);
	GetWorld()->GetTimerManager().SetTimer(StopAoETimer, this, &AHyperSlashDashAttack::StopAttack, StopAoETime, false);
}

void AHyperSlashDashAttack::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(StartAoETimer);
	GetWorld()->GetTimerManager().ClearTimer(StopAoETimer);
}

void AHyperSlashDashAttack::StartAttack()
{
	// raise the active flag
	bIsAoEActive = true;

	// find all actors overlapping the NPC
	TArray<AActor*> Overlaps;
	CollisionCapsule->GetOverlappingActors(Overlaps, AHyperSlashEnemy::StaticClass());

	// process each overlapping actor
	for (AActor* Current : Overlaps)
	{
		if (AHyperSlashEnemy* Enemy = Cast<AHyperSlashEnemy>(Current))
		{
			// tell the NPC it's been hit
			Enemy->ProjectileImpact(FVector::ZeroVector);
		}
	}
}

void AHyperSlashDashAttack::StopAttack()
{
	bIsAoEActive = false;

	GetWorld()->GetTimerManager().ClearTimer(StartAoETimer);
	GetWorld()->GetTimerManager().ClearTimer(StopAoETimer);

	Destroy();
}

void AHyperSlashDashAttack::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// is the explosion active?
	if (bIsAoEActive)
	{
		// did we overlap an NPC?
		if (AHyperSlashEnemy* Enemy = Cast<AHyperSlashEnemy>(OtherActor))
		{
			// tell the NPC it's been hit
			Enemy->ProjectileImpact(FVector::ZeroVector);
		}
	}
}
