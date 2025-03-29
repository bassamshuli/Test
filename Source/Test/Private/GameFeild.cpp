// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeild.h"
#include "Engine/World.h"
#include "Tile.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AGameFeild::AGameFeild()
{
    PrimaryActorTick.bCanEverTick = false;

    Rows = 25;
    Columns = 25;
    CellSize = 37.0f;

    static ConstructorHelpers::FClassFinder<AActor> TileBP(TEXT("/Game/Blueprints/BP_Tile"));
    if (TileBP.Succeeded()) TileBlueprint = TileBP.Class;

    static ConstructorHelpers::FClassFinder<AObstacles> MountainBP(TEXT("/Game/Blueprints/BP_Mountain"));
    if (MountainBP.Succeeded()) MountainBlueprint = MountainBP.Class;

    static ConstructorHelpers::FClassFinder<AObstacles> TreeBP(TEXT("/Game/Blueprints/BP_Tree"));
    if (TreeBP.Succeeded()) TreeBlueprint = TreeBP.Class;

    ObstacleToSpawn = TreeBlueprint;
}

void AGameFeild::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    GenerateGrid();
}

void AGameFeild::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->bEnableClickEvents = true;
        PC->bEnableMouseOverEvents = true;
        PC->bShowMouseCursor = true;
    }

    GenerateObstacles();

    if (GameWidgetClass)
    {
        GameWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), GameWidgetClass);
        if (GameWidgetInstance)
        {
            GameWidgetInstance->AddToViewport();
        }
    }

    SpawnQueue = { BP_Brawler_Green, BP_Brawler_Red, BP_Sniper_Green, BP_Sniper_Red };
    CurrentUnitIndex = 0;
    bIsPlayerTurn = true;
    bGameStarted = true;
}

void AGameFeild::GenerateGrid()
{
    if (!TileBlueprint) return;

    UWorld* World = GetWorld();
    if (!World) return;

    Tiles.Empty();
    float Spacing = 5.0f;
    float AdjustedCellSize = CellSize + Spacing;
    float StartX = -((Columns - 1) * AdjustedCellSize) / 2.0f;
    float StartY = -((Rows - 1) * AdjustedCellSize) / 2.0f;

    for (int32 Row = 0; Row < Rows; ++Row)
    {
        for (int32 Col = 0; Col < Columns; ++Col)
        {
            FVector Location(StartX + Col * AdjustedCellSize, StartY + Row * AdjustedCellSize, 0);
            AActor* SpawnedTile = World->SpawnActor<AActor>(TileBlueprint, Location, FRotator::ZeroRotator);
            if (ATile* Tile = Cast<ATile>(SpawnedTile))
            {
                Tiles.Add(Tile);
            }
        }
    }
}

void AGameFeild::GenerateObstacles()
{
    if (!MountainBlueprint || !TreeBlueprint || Tiles.Num() == 0) return;
    UWorld* World = GetWorld();
    int32 NumObstacles = FMath::RoundToInt(Rows * Columns * 0.1f);
    TSet<int32> UsedIndices;

    while (UsedIndices.Num() < NumObstacles)
    {
        int32 Index = FMath::RandRange(0, Tiles.Num() - 1);
        if (!UsedIndices.Contains(Index))
        {
            UsedIndices.Add(Index);
            FVector Location = Tiles[Index]->GetActorLocation() + FVector(0, 0, 10);
            TSubclassOf<AObstacles> Obstacle = FMath::RandBool() ? MountainBlueprint : TreeBlueprint;
            World->SpawnActor<AObstacles>(Obstacle, Location, FRotator::ZeroRotator);
        }
    }
}

void AGameFeild::HandleTileClicked(ATile* ClickedTile)
{
    if (!ClickedTile || !ClickedTile->IsTileFree()) return;
    if (!bIsPlayerTurn || CurrentUnitIndex >= SpawnQueue.Num()) return;

    TSubclassOf<ASoldier> UnitToSpawn = SpawnQueue[CurrentUnitIndex];
    FVector Location = ClickedTile->GetActorLocation() + FVector(0, 0, 50);

    ASoldier* NewUnit = GetWorld()->SpawnActor<ASoldier>(UnitToSpawn, Location, FRotator::ZeroRotator);
    if (NewUnit)
    {
        ClickedTile->SetTileOccupied(true);
        CurrentUnitIndex++;
        bIsPlayerTurn = false;
        GetWorldTimerManager().SetTimerForNextTick(this, &AGameFeild::PlaceAIUnit);
    }
}

void AGameFeild::PlaceAIUnit()
{
    if (CurrentUnitIndex >= SpawnQueue.Num()) return;

    // 🔹 Filtra tutte le tile libere
    TArray<ATile*> FreeTiles;
    for (ATile* Tile : Tiles)
    {
        if (Tile && Tile->IsTileFree())
        {
            FreeTiles.Add(Tile);
        }
    }

    // 🔹 Se ci sono tile libere, scegli una a caso
    if (FreeTiles.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, FreeTiles.Num() - 1);
        ATile* SelectedTile = FreeTiles[RandomIndex];

        FVector SpawnLocation = SelectedTile->GetActorLocation() + FVector(0, 0, 50);
        ASoldier* AIUnit = GetWorld()->SpawnActor<ASoldier>(SpawnQueue[CurrentUnitIndex], SpawnLocation, FRotator::ZeroRotator);

        if (AIUnit)
        {
            SelectedTile->SetTileOccupied(true);
            CurrentUnitIndex++;
            bIsPlayerTurn = true;
        }
    }
}

void AGameFeild::StartGame()
{
    bGameStarted = true;
}

void AGameFeild::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
