// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "BaseGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ATile::ATile()
{
    PrimaryActorTick.bCanEverTick = false;

    // Crea e configura il componente mesh
    TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
    RootComponent = TileMesh;
    TileMesh->SetMobility(EComponentMobility::Static);
    TileMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    TileMesh->SetRelativeScale3D(FVector(3.0f, 3.0f, 0.1f));

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMat(TEXT("/Game/Materials/M_Tile_Default.M_Tile_Default"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> SelectedMat(TEXT("/Game/Materials/M_Tile_Selected.M_Tile_Selected"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> EnemyMat(TEXT("/Game/Materials/M_Tile_Enemy.M_Tile_Enemy"));

    if (DefaultMat.Succeeded()) DefaultMaterial = DefaultMat.Object;
    if (SelectedMat.Succeeded()) SelectedMaterial = SelectedMat.Object;
    if (EnemyMat.Succeeded()) EnemyMaterial = EnemyMat.Object;

    // Imposta la mesh base: cubo
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (MeshAsset.Succeeded())
    {
        TileMesh->SetStaticMesh(MeshAsset.Object);
    }
}

void ATile::BeginPlay()
{
    Super::BeginPlay();

    // Abilita il click
    OnClicked.AddUniqueDynamic(this, &ATile::OnTileClicked);

    // Applica il materiale di default
    if (TileMesh && DefaultMaterial)
    {
        TileMesh->SetMaterial(0, DefaultMaterial);
    }
}

void ATile::OnTileClicked(AActor* TouchedActor, FKey ButtonPressed)
{
    UE_LOG(LogTemp, Warning, TEXT("🖱️ Tile cliccata: %s"), *GetName());
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
    if (TileMesh)
    {
        UMaterialInterface* MaterialToApply = bSelected ? SelectedMaterial : DefaultMaterial;
        TileMesh->SetMaterial(0, MaterialToApply);
    }
}

void ATile::SetEnemyHighlighted(bool bHighlight)
{
    if (TileMesh)
    {
        if (bHighlight && EnemyMaterial)
        {
            TileMesh->SetMaterial(0, EnemyMaterial);
        }
        else if (DefaultMaterial)
        {
            TileMesh->SetMaterial(0, DefaultMaterial);
        }
    }
}

void ATile::ResetTile()
{
    bIsOccupied = false;
    bHasObstacle = false;
    SetSelected(false);
    SetEnemyHighlighted(false);

    if (TileMesh && DefaultMaterial)
    {
        TileMesh->SetMaterial(0, DefaultMaterial);
    }
}