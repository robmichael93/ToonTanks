// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"

#define OUT

// Constructor
ATank::ATank()
{
SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
SpringArm->SetupAttachment(RootComponent);

Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
    TankPlayerController = Cast<APlayerController>(GetController());
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ATank::Move);
    PlayerInputComponent->BindAxis("Turn", this, &ATank::Turn);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATank::Fire);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (TankPlayerController)
    {
        FHitResult Hit;
        TankPlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility,
            false,
            OUT Hit
        );
        FVector HitLocation = Hit.ImpactPoint;

        RotateTurret(HitLocation, DeltaTime);
    }
}

void ATank::Move(float Value)
{
    FVector DeltaLocation(0.f);
    DeltaLocation.X = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * Speed;
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
    FRotator DeltaRotation(0.f);
    DeltaRotation.Yaw = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnRate;
    AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bAlive = false;
}