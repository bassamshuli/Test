// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFeild.h"

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
    AGameFeild* Game = Cast<AGameFeild>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameFeild::StaticClass()));
    if (Game)
    {
        Game->HandleTileClicked(this);
    }
}

bool ATile::IsTileFree() const { return !bIsOccupied; }
void ATile::SetTileOccupied(bool bOccupied) { bIsOccupied = bOccupied; }
