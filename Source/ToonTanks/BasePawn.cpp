// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Projectile.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::RotateTurret(FVector LookAtTarget, float DeltaTime)
{
    FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	LookAtRotation = FMath::Lerp(TurretMesh->GetComponentRotation(), LookAtRotation, DeltaTime * TurretRotateSpeed);
    TurretMesh->SetWorldRotation(LookAtRotation);
}

void ABasePawn::Fire()
{
	FVector ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	// DrawDebugSphere(
	// 	GetWorld(), 
	// 	ProjectileSpawnLocation, 
	// 	10.f, 
	// 	32, 
	// 	FColor::Red,
	// 	false,
	// 	3.f);

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass, 
		ProjectileSpawnLocation,
		ProjectileSpawnPoint->GetComponentRotation()
	);
	Projectile->SetOwner(this);
}

void ABasePawn::HandleDestruction()
{
	// Visual/Sound effects with death
	if (Explosion)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, GetActorLocation(), GetActorRotation());
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(DeathCameraShakeClass);
	}
}