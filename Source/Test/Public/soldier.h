// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "soldier.generated.h"

UCLASS()
class TEST_API Asoldier : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	Asoldier();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Attributi comuni a tutti i soldati
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 AttackDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    int32 MovementRange;

    // Indica se il soldato è controllato dal giocatore o dall'IA
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
    bool bIsPlayerControlled;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 🔹 Imposta se il soldato è del giocatore o dell'IA
    void SetPlayerControlled(bool bIsPlayer) { bIsPlayerControlled = bIsPlayer; }

    virtual void MoveTo(FVector TargetLocation);
    virtual void Attack(Asoldier* Target);
    virtual void TakeDamage(int32 DamageAmount);

    UFUNCTION(BlueprintCallable)
    bool IsAlive() const { return Health > 0; }

};
