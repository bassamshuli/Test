// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class TEST_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// 🔹 Imposta la posizione della cella nella griglia
	void SetGridPosition(FVector2D NewPosition);

	// 🔹 Controlla se la cella è occupata
	bool IsOccupied() const;

	// 🔹 Imposta un'unità nella cella
	void SetOccupyingUnit(AActor* NewUnit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector2D GridPosition;  // 🔹 Posizione della cella nella griglia

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
	AActor* OccupyingUnit;  // 🔹 Unità presente nella cella (se c'è)

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
