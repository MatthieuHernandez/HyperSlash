#include "HyperSlashCharacter.h"
#include "HyperSlashGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Weapon.h"

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
        FActorSpawnParameters spawnParams;
        spawnParams.Owner = this;
        spawnParams.Instigator = this;
        equippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
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
    if (CanAct())
    {
        if (WantPerformTeleportation)
        {
            PerformTeleportation();
            WantPerformTeleportation = false;
        }
        else if (WantPerformCircularAttack)
        {
            PerformCircularAttack();
            WantPerformCircularAttack = false;
        }
        else if (WantPerformDashAttack)
        {
            PerformDashAttack();
            WantPerformDashAttack = false;
        }
    }
}

void AHyperSlashCharacter::PlayCircularAttackAnimation()
{
    if (auto* animInstance = GetMesh()->GetAnimInstance())
    {
        animInstance->Montage_Play(CircularAttackAnimation);
    }
    if (SlashSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, SlashSound, GetActorLocation(), 1.0f, 1.0f, 0.0f);
    }
}

void AHyperSlashCharacter::EndCircularAttack()
{
    UpdateScoreEndAttack();
    equippedWeapon->DisableHitbox();
    isAttacking = false;
}

void AHyperSlashCharacter::PlayDashAttackAnimation()
{
    if (auto* animInstance = GetMesh()->GetAnimInstance())
    {
        animInstance->PlaySlotAnimationAsDynamicMontage(DashAttackAnimation, FName("DefaultSlot"), 0.15f, 0.15f, DashSpeed);
    }
}

void AHyperSlashCharacter::PlayTeleportationAnimation()
{
    if (auto* animInstance = GetMesh()->GetAnimInstance())
    {
        animInstance->Montage_Play(TeleportationAnimation);
    }
}

void AHyperSlashCharacter::PerformCircularAttack()
{
    UpdateScoreStartAttack();
    PlayCircularAttackAnimation();

    const float attackDuration = CircularAttackAnimation->GetSectionLength(0);
    isAttacking = true;
    equippedWeapon->EnableHitbox();
    FTimerHandle timer;
    GetWorldTimerManager().SetTimer(timer, this, &AHyperSlashCharacter::EndCircularAttack, attackDuration, false);
}

void AHyperSlashCharacter::PerformDashAttack()
{
    UpdateScoreStartAttack();
    PlayDashAttackAnimation();

    float dashDuration = DashAttackAnimation->GetPlayLength() / DashSpeed;

    dashAttackVector = GetActorForwardVector();
    dashAttackVector.Z = 0.f;
    dashAttackVector.Normalize();
    dashAttackVector *= DashDistance * dashDuration * DashSpeed;

    isDashing = true;
    equippedWeapon->EnableHitbox();
    FTimerHandle timer;
    GetWorldTimerManager().SetTimer(timer, this, &AHyperSlashCharacter::EndDashAttack, dashDuration, false);
}

void AHyperSlashCharacter::PerformTeleportation()
{
    PlayTeleportationAnimation();
    const float teleportationDuration = TeleportationAnimation->GetSectionLength(0);
    isTeleporting = true;
    FTimerHandle timer;
    GetWorldTimerManager().SetTimer(timer, [this]() { isTeleporting = false; }, teleportationDuration, false);
}

void AHyperSlashCharacter::EndDashAttack()
{
    UpdateScoreEndAttack();
    equippedWeapon->DisableHitbox();
    isDashing = false;
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
    FTimerHandle actTimer;
    GetWorldTimerManager().SetTimer(actTimer, [this]() {canAct = true; }, 1.0f, false);
    FTimerHandle hitTimer;
    GetWorldTimerManager().SetTimer(hitTimer, [this]() {canBeHit = true; }, 1.6f, false);
}

void AHyperSlashCharacter::UpdateScoreStartAttack()
{
    scoreMultiplier++;
    OnScoreChanged.Broadcast(score, scoreMultiplier);
}

void AHyperSlashCharacter::UpdateScoreEndAttack()
{
    if (numberOfEnemyKilledByPreviousAttack == 0)
    {
        scoreMultiplier = (scoreMultiplier / 2);
    }
    numberOfEnemyKilledByPreviousAttack = 0;
    OnScoreChanged.Broadcast(score, scoreMultiplier);
}

void AHyperSlashCharacter::EnemyKilled()
{
    numberOfEnemyKilledByPreviousAttack++;
    score += (numberOfEnemyKilledByPreviousAttack + 1) * scoreMultiplier;
    OnScoreChanged.Broadcast(score, scoreMultiplier);
}

void AHyperSlashCharacter::Die()
{
    if (auto* GM = Cast<AHyperSlashGameMode>(GetWorld()->GetAuthGameMode()))
    {
        FTimerHandle timer;
        GetWorldTimerManager().SetTimer(timer, GM, &AHyperSlashGameMode::GameOver, 1.2f, false);
    }
}

bool AHyperSlashCharacter::CanAct() const
{
    return canAct && !isDashing && !isAttacking && !isTeleporting;
}

int32 AHyperSlashCharacter::GetScore() const
{
    return score;
}
