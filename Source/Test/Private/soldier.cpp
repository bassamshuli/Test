// Fill out your copyright notice in the Description page of Project Settings.


#include "soldier.h"

// Sets default values
Asoldier::Asoldier()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsPlayerControlled = true;  // Default: soldato del giocatore

}
// 🔹 Metodo per muovere il soldato verso una posizione target
void Asoldier::MoveTo(FVector TargetLocation)
{
	SetActorLocation(TargetLocation);
}
// 🔹 Metodo per attaccare un altro soldato
void Asoldier::Attack(Asoldier* Target)
{
    if (!Target || !IsAlive()) return;

    float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());

    // Controlliamo se il bersaglio è a distanza di attacco
    if (Distance <= AttackRange * 100.0f)
    {
        Target->TakeDamage(AttackDamage);
    }
}
// 🔹 Metodo per ricevere danni
void Asoldier::TakeDamage(int32 DamageAmount)
{
    Health -= DamageAmount;
    if (Health <= 0)
    {
        Health = 0;
        Destroy();
    }
}

// Called when the game starts or when spawned
void Asoldier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Asoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Asoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

