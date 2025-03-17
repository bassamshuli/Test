// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerSoldier.h"

// Sets default values
ABrawlerSoldier::ABrawlerSoldier()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 120;
	AttackDamage = 10;
	AttackRange = 1;
	MovementRange = 4;
}
// 🔹 Metodo per muovere l'unità
void ABrawlerSoldier::MoveTo(FVector TargetLocation)
{
    SetActorLocation(TargetLocation);
}

// 🔹 Metodo per attaccare un altro soldato
void ABrawlerSoldier::Attack(ABrawlerSoldier* Target)
{
    if (!Target || !IsAlive()) return;

    float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());

    if (Distance <= AttackRange * 100.0f)
    {
        Target->TakeDamage(AttackDamage);
    }
}

// 🔹 Metodo per ricevere danni
void ABrawlerSoldier::TakeDamage(int32 DamageAmount)
{
    Health -= DamageAmount;
    if (Health <= 0)
    {
        Health = 0;
        Destroy();
    }
}
// Called when the game starts or when spawned
void ABrawlerSoldier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrawlerSoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABrawlerSoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

