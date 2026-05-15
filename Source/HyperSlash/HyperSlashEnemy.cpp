// Fill out your copyright notice in the Description page of Project Settings.


#include "HyperSlashEnemy.h"
#include "HyperSlashGameMode.h"
#include "HyperSlashCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StateTreeAIComponent.h"
#include "Engine/World.h"
#include "TwinStickNPCDestruction.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

// Sets default values
AHyperSlashEnemy::AHyperSlashEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// ensure we spawn an AI controller when we're spawned
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// configure the inherited components
	GetCapsuleComponent()->SetCapsuleRadius(45.0f);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->MaxAcceleration = 1000.0f;
	GetCharacterMovement()->BrakingFriction = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 250.0f;
	GetCharacterMovement()->AvoidanceWeight = 1.0f;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
}

// Called when the game starts or when spawned
void AHyperSlashEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (AHyperSlashGameMode* GM = Cast<AHyperSlashGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->IncreaseEnemyCount();
	}
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AHyperSlashEnemy::OnHit);
}

void AHyperSlashEnemy::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(DestructionTimer);
}

void AHyperSlashEnemy::Destroyed()
{
	if (AHyperSlashGameMode* GM = Cast<AHyperSlashGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->DecreaseEnemyCount();
	}
	Super::Destroyed();
}

void AHyperSlashEnemy::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	// have we collided against the player?
	if (AHyperSlashCharacter* PlayerCharacter = Cast<AHyperSlashCharacter>(Other))
	{
		// apply damage to the character
		// PlayerCharacter->HandleDamage(1.0f, GetActorForwardVector());
	}
}

void AHyperSlashEnemy::ProjectileImpact(const FVector& ForwardVector)
{
	// only handle damage if we haven't been hit yet
	if (bHit)
	{
		return;
	}

	// raise the hit flag
	bHit = true;

	// deactivate character movement
	GetCharacterMovement()->Deactivate();

	// spawn the NPC destruction proxy
	ATwinStickNPCDestruction* DestructionProxy = GetWorld()->SpawnActor<ATwinStickNPCDestruction>(DestructionProxyClass, GetActorTransform());

	// hide this actor
	SetActorHiddenInGame(true);

	// disable collision
	SetActorEnableCollision(false);

	// defer destruction
	GetWorld()->GetTimerManager().SetTimer(DestructionTimer, this, &AHyperSlashEnemy::DeferredDestroy, DeferredDestructionTime, false);
}

void AHyperSlashEnemy::StopStateTreeLogic()
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UStateTreeAIComponent* StateTreeComponent = AIController->FindComponentByClass<UStateTreeAIComponent>())
		{
			StateTreeComponent->StopLogic(TEXT("Enemy destroyed"));
		}
	}
}

void AHyperSlashEnemy::DeferredDestroy()
{
	StopStateTreeLogic();
	Destroy();
}

Direction AHyperSlashEnemy::GetHitDirection(AHyperSlashCharacter* Player)
{
	FVector DirectionToEnemy =
		(GetActorLocation() - Player->GetActorLocation()).GetSafeNormal();
	FVector PlayerForward = Player->GetActorForwardVector();
	FVector PlayerRight = Player->GetActorRightVector();
	float ForwardDot = FVector::DotProduct(PlayerForward, DirectionToEnemy);
	float RightDot = FVector::DotProduct(PlayerRight, DirectionToEnemy);

	if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
	{
		if (ForwardDot > 0.0f) return Direction::Front;
		else return Direction::Back;
	}
	else
	{
		if (RightDot > 0.0f) return Direction::Right;
		else return Direction::Left;
	}
}

void AHyperSlashEnemy::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	auto* Player = Cast<AHyperSlashCharacter>(OtherActor);
	if (Player)
	{
		Direction Dir = GetHitDirection(Player);
		Player->BeHit(Dir);
	}
}
