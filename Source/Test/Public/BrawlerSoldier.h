﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BrawlerSoldier.generated.h"

UCLASS()
class TEST_API ABrawlerSoldier : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABrawlerSoldier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 MovementRange;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 🔹 Metodi indipendenti
	void MoveTo(FVector TargetLocation);
	void Attack(ABrawlerSoldier* Target);
	void TakeDamage(int32 DamageAmount);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const { return Health > 0; }
};
