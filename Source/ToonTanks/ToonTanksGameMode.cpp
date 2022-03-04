// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "ToonTanksGameMode.h"
#include "ToonTanksPlayerController.h"
#include "Turret.h"

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();
    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
    NumberOfTurrets = GetNumberOfTurrets();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController, 
            &AToonTanksPlayerController::SetPlayerEnabledState, 
            true
        );
        GetWorldTimerManager().SetTimer(
            PlayerEnableTimerHandle,
            PlayerEnableTimerDelegate,
            StartDelay,
            false
        );
    }
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    if (DeadActor == Tank)
    {
        Tank->HandleDestruction();
        if (ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    } else if (ATurret* DestroyedTurret = Cast<ATurret>(DeadActor))
    {
        DestroyedTurret->HandleDestruction();
        NumberOfTurrets--;
        if (NumberOfTurrets == 0)
        {
            GameOver(true);
        }
    }
}

int32 AToonTanksGameMode::GetNumberOfTurrets()
{
    TArray<AActor*> Turrets = {};
    UGameplayStatics::GetAllActorsOfClass(this, ATurret::StaticClass(), Turrets);
    return Turrets.Num();
}