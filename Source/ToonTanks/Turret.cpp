// Fill out your copyright notice in the Description page of Project Settings.

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Turret.h"

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    TurretRotateSpeed = 7.5f;

    GetWorldTimerManager().SetTimer(
        FireRateTimerHandle, 
        this, 
        &ATurret::CheckFireCondition, 
        FireRate, 
        true
    );
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (InFireRange())
    {
        RotateTurret(Tank->GetActorLocation(), DeltaTime);
    }
}

bool ATurret::InFireRange()
{
    if (Tank)
    {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

        if (Distance <= FireRange)
        {
            return true;
        }
    }
    return false;
}

void ATurret::CheckFireCondition()
{
    if (Tank == nullptr) { return; }
    
    if (InFireRange() && Tank->IsAlive())
    {
        Fire();
    }
}

void ATurret::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}