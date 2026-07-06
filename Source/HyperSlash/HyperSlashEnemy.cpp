#include "HyperSlashEnemy.h"
#include "HyperSlashGameMode.h"
#include "HyperSlashCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StateTreeAIComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AHyperSlashEnemy::AHyperSlashEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

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
    if (auto* Player = UGameplayStatics::GetPlayerPawn(this, 0))
    {
        FVector Direction = Player->GetActorLocation() - GetActorLocation();
        Direction.Z = 0.0f;
        SetActorRotation(Direction.Rotation());
    }
    if (AHyperSlashGameMode* GM = Cast<AHyperSlashGameMode>(GetWorld()->GetAuthGameMode()))
    {
        GM->IncreaseEnemyCount();
    }
    GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AHyperSlashEnemy::OnHit);
    if (DigUpDirt)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DigUpDirt, GetActorLocation());
    }
}

void AHyperSlashEnemy::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    GetCharacterMovement()->bUseRVOAvoidance = false;
    GetWorld()->GetTimerManager().ClearTimer(DestructionTimer);
    if (auto* AI = Cast<AAIController>(GetController()))
    {
        AI->StopMovement();
    }
    Super::EndPlay(EndPlayReason);
}

void AHyperSlashEnemy::Destroyed()
{
    if (AHyperSlashGameMode* GM = Cast<AHyperSlashGameMode>(GetWorld()->GetAuthGameMode()))
    {
        GM->DecreaseEnemyCount();
    }
    Super::Destroyed();
}

void AHyperSlashEnemy::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (CanAct())
    {
        if (AAIController* AI = Cast<AAIController>(GetController()))
        {
            if (auto* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
            {
                AI->MoveToActor(player, 50.0f);
            }
        }
    }
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

void AHyperSlashEnemy::GetHit(const FVector& Knockback)
{
    if (!wasHitRecently)
    {
        LaunchCharacter(Knockback * 1000.0f, true, false);
        Health--;
        if (Health <= 0)
        {
            Die();
        }
        wasHitRecently = true;
        FTimerHandle timer;
        GetWorldTimerManager().SetTimer(timer, [this]() { wasHitRecently = false; }, 0.4f, false);
    }
}

void AHyperSlashEnemy::Die()
{
    GetCharacterMovement()->Deactivate();
    if (DeathSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
    }
    if (BloodSpurt)
    {
        auto EffectRotation = GetActorRotation();
        EffectRotation.Yaw += 180.0f;
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSpurt, GetActorLocation(), EffectRotation);
    }

    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    GetWorld()->GetTimerManager().SetTimer(DestructionTimer, this, &AHyperSlashEnemy::DeferredDestroy, DeferredDestructionTime, false);

    // Notify the player
    if (auto* Player = Cast<AHyperSlashCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {
        Player->EnemyKilled();
    }
}

void AHyperSlashEnemy::DeferredDestroy()
{
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

void AHyperSlashEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    auto* player = Cast<AHyperSlashCharacter>(OtherActor);
    if (player)
    {
        auto direction = GetHitDirection(player);
        player->BeHit(direction);
    }
}

bool AHyperSlashEnemy::CanAct() const
{
    return canAct && !wasHitRecently;
}