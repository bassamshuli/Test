// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "BaseGameMode.h"
#include "Components/StaticMeshComponent.h"

ATile::ATile()
{
    PrimaryActorTick.bCanEverTick = false;
    bIsOccupied = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    // ❌ NON creiamo TileMesh qui
}

void ATile::BeginPlay()
{
    Super::BeginPlay();
    OnClicked.AddUniqueDynamic(this, &ATile::OnTileClicked);

    UStaticMeshComponent* Mesh = FindComponentByClass<UStaticMeshComponent>();
    if (Mesh && DefaultMaterial)
    {
        Mesh->SetMaterial(0, DefaultMaterial);
        UE_LOG(LogTemp, Warning, TEXT("🟫 Tile %s inizializzato con materiale di default."), *GetName());
    }
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

bool ATile::IsTileFree() const
{
    return !bIsOccupied && !bHasObstacle;
}

void ATile::SetTileOccupied(bool bOccupied)
{
    bIsOccupied = bOccupied;
}

void ATile::SetSelected(bool bSelected)
{
    UStaticMeshComponent* Mesh = FindComponentByClass<UStaticMeshComponent>();
    if (Mesh)
    {
        UMaterialInterface* MaterialToApply = bSelected ? SelectedMaterial : DefaultMaterial;
        Mesh->SetMaterial(0, MaterialToApply);
        UE_LOG(LogTemp, Warning, TEXT("🟩 Tile %s evidenziata: %s"), *GetName(), bSelected ? TEXT("SI") : TEXT("NO"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ StaticMeshComponent NON trovato in %s"), *GetName());
    }
}