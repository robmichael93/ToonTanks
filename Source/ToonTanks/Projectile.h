// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UParticleSystemComponent* SmokeTrailParticles = nullptr;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float InitialSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Damage = 25.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* HitParticles = nullptr;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* LaunchSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
