#include "HyperSlashAttack.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HyperSlashEnemy.h"

// Sets default values
AHyperSlashAttack::AHyperSlashAttack()
{
	PrimaryActorTick.bCanEverTick = true;

	// create the root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// create the mesh that provides the visual representation for the AoE
	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere Visual"));
	SphereVisual->SetupAttachment(RootComponent);

	SphereVisual->SetCollisionProfileName(FName("NoCollision"));

	// create the collision sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetupAttachment(RootComponent);

	CollisionSphere->SetNotifyRigidBodyCollision(true);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AHyperSlashAttack::OnAttackOverlap);
}

// Called when the game starts or when spawned
void AHyperSlashAttack::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(StartAoETimer, this, &AHyperSlashAttack::StartAttack, StartAoETime, false);
	GetWorld()->GetTimerManager().SetTimer(StopAoETimer, this, &AHyperSlashAttack::StopAttack, StopAoETime, false);
}

void AHyperSlashAttack::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(StartAoETimer);
	GetWorld()->GetTimerManager().ClearTimer(StopAoETimer);
}

void AHyperSlashAttack::StartAttack()
{
	// raise the active flag
	bIsAoEActive = true;

	// find all actors overlapping the NPC
	TArray<AActor*> Overlaps;
	CollisionSphere->GetOverlappingActors(Overlaps, AHyperSlashEnemy::StaticClass());

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

void AHyperSlashAttack::StopAttack()
{
	bIsAoEActive = false;

	GetWorld()->GetTimerManager().ClearTimer(StartAoETimer);
	GetWorld()->GetTimerManager().ClearTimer(StopAoETimer);

	Destroy();
}

void AHyperSlashAttack::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
