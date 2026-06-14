#include "HyperSlashCharacter.h"
#include "HyperSlashGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"

AHyperSlashCharacter::AHyperSlashCharacter()
{
    // Set size for player capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    GetCapsuleComponent()->SetGenerateOverlapEvents(true);

    // Don't rotate character to camera direction
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = false;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void AHyperSlashCharacter::BeginPlay()
{
    Super::BeginPlay();
    scoreMultiplier = 1;
    score = 0;
    numberOfEnemyKilledByPreviousAttack = 0;
    AActor* TargetCamera = nullptr;
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
        {
            if (It->ActorHasTag("GameplayCamera"))
            {
                TargetCamera = *It;
                break;
            }
        }
        if (TargetCamera)
        {
            PC->SetViewTargetWithBlend(TargetCamera, 0.5f);
        }
    }
    if (WeaponClass)
    {
        equippedWeapon = GetWorld()->SpawnActor<AActor>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (equippedWeapon)
        {
            equippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_socket_r"));
        }
    }
}

void AHyperSlashCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (isDashing)
    {
        AddActorWorldOffset(dashAttackVector * DeltaSeconds, true);
    }
}

void AHyperSlashCharacter::PlayAttackAnimation()
{
    if (!AttackAnimation) return;
    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        AnimInstance->PlaySlotAnimationAsDynamicMontage(AttackAnimation, FName("DefaultSlot"));
    }
}

void AHyperSlashCharacter::PlayDashAttackAnimation()
{
    if (!AttackAnimation) return;
    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        AnimInstance->PlaySlotAnimationAsDynamicMontage(DashAttackAnimation, FName("DefaultSlot"));
    }
}

void AHyperSlashCharacter::SpawnAttack()
{
    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Instigator = this;

    GetWorld()->SpawnActor<AHyperSlashAttack>(
        AttackClass,
        GetActorLocation(),
        GetActorRotation(),
        Params);
}

void AHyperSlashCharacter::SpawnDashAttack()
{
    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Instigator = this;

    GetWorld()->SpawnActor<AHyperSlashDashAttack>(
        DashAttackClass,
        GetActorLocation(),
        GetActorRotation(),
        Params);

    float dashDuration = DashAttackAnimation->GetPlayLength();

    dashAttackVector = GetActorForwardVector();
    dashAttackVector.Z = 0.f;
    dashAttackVector.Normalize();
    dashAttackVector *= 500.0f * dashDuration;

    isDashing = true;
    GetWorldTimerManager().SetTimer(dashTimer, [this]() {isDashing = false; }, dashDuration, false);
}

void AHyperSlashCharacter::PerformAttack()
{
    if (!CanAct()) return;
    PlayAttackAnimation();
    SpawnAttack();
    Attack();
}

void AHyperSlashCharacter::PerformDashAttack()
{
    if (!CanAct()) return;
    PlayDashAttackAnimation();
    SpawnDashAttack();
    Attack();
}

void AHyperSlashCharacter::BeHit(Direction D)
{
    if (!canBeHit) return;
    canBeHit = false;
    canAct = false;
    Health--;
    if (Health <= 0)
    {
        Die();
    }
    UAnimSequence* AnimSeq = nullptr;
    switch (D)
    {
    case Direction::Front:
        AnimSeq = HitFrontAnnimation;
        break;
    case Direction::Back:
        AnimSeq = HitBackAnnimation;
        break;
    case Direction::Left:
        AnimSeq = HitLeftAnnimation;
        break;
    case Direction::Right:
        AnimSeq = HitRightAnnimation;
        break;
    default:
        break;
    }
    if (AnimSeq) 
    {
        if (auto* AnimInstance = GetMesh()->GetAnimInstance())
        {
            AnimInstance->PlaySlotAnimationAsDynamicMontage(AnimSeq, FName("DefaultSlot"));
        }
    }
    GetWorldTimerManager().SetTimer(actTimer, [this]() {canAct = true; }, 1.0f, false);
    GetWorldTimerManager().SetTimer(hitTimer, [this]() {canBeHit = true; }, 1.6f, false);
}

void AHyperSlashCharacter::Attack()
{
    if (numberOfEnemyKilledByPreviousAttack == 0)
    {
        scoreMultiplier = (scoreMultiplier / 2);
    }
    scoreMultiplier++;
    numberOfEnemyKilledByPreviousAttack = 0;
}

void AHyperSlashCharacter::EnemyKilled() {
    numberOfEnemyKilledByPreviousAttack++;
    score += numberOfEnemyKilledByPreviousAttack * scoreMultiplier;
    OnScoreChanged.Broadcast(score, scoreMultiplier);
}

void AHyperSlashCharacter::Die() 
{
    if (auto* GM = Cast<AHyperSlashGameMode>(GetWorld()->GetAuthGameMode()))
    {
        GetWorldTimerManager().SetTimer(dieTimer, GM, &AHyperSlashGameMode::GameOver, 1.2f, false);
    }
}

bool AHyperSlashCharacter::CanAct() const
{
    return canAct && !isDashing;
}