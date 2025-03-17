// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperSoldier.h"

// Sets default values
ASniperSoldier::ASniperSoldier()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 80;
	AttackDamage = 15;
	AttackRange = 5;
	MovementRange = 3;

}
// 🔹 Metodo per muovere l'unità
void ASniperSoldier::MoveTo(FVector TargetLocation)
{
	SetActorLocation(TargetLocation);
}
// 🔹 Metodo per attaccare un altro soldato
void ASniperSoldier::Attack(ASniperSoldier* Target)
{
	if (!Target || !IsAlive()) return;

	float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());

	if (Distance <= AttackRange * 100.0f)
	{
		Target->TakeDamage(AttackDamage);
	}
}
// 🔹 Metodo per ricevere danni
void ASniperSoldier::TakeDamage(int32 DamageAmount)
{
	Health -= DamageAmount;
	if (Health <= 0)
	{
		Health = 0;
		Destroy();
	}
}

// Called when the game starts or when spawned
void ASniperSoldier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASniperSoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASniperSoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

