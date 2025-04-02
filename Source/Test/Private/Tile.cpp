// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "BaseGameMode.h"

ATile::ATile()
{
    PrimaryActorTick.bCanEverTick = false;
    bIsOccupied = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ATile::BeginPlay()
{
    Super::BeginPlay();
    OnClicked.AddUniqueDynamic(this, &ATile::OnTileClicked);
}

void ATile::OnTileClicked(AActor* TouchedActor, FKey ButtonPressed)
{
    if (bIsOccupied || bHasObstacle) return;

    ABaseGameMode* GameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode && GameMode->bIsPlayerTurn)
    {
        GameMode->HandleTileClicked(this);
    }
}

bool ATile::IsTileFree() const { return !bIsOccupied && !bHasObstacle; }
void ATile::SetTileOccupied(bool bOccupied) { bIsOccupied = bOccupied; }
