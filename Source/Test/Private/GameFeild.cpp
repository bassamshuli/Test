// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeild.h"
#include "Tile.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Mountain.h"
#include "Tree.h"
#include "EngineUtils.h"

AGameFeild::AGameFeild()
{
    PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FClassFinder<AActor> TileBP(TEXT("/Game/Blueprints/BP_FeildAndMode/BP_Tile"));
    if (TileBP.Succeeded()) TileBlueprint = TileBP.Class;

    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/Blueprints/BP_Widgets/WBP_Game"));
    if (WidgetClassFinder.Succeeded()) GameWidgetClass = WidgetClassFinder.Class;

    // Usa le classi C++ direttamente
    MountainBlueprint = AMountain::StaticClass();
    TreeBlueprint = ATree::StaticClass();
    ObstacleToSpawn = TreeBlueprint;
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

    GenerateGrid();
    GenerateObstacles();

    if (GameWidgetClass)
    {
        GameUIInstance = Cast<UWBP_Game>(CreateWidget<UUserWidget>(GetWorld(), GameWidgetClass));
        if (GameUIInstance)
        {
            GameUIInstance->AddToViewport();
            GameUIInstance->ShowWelcomeMessage();
        }
    }

    SpawnQueue = { BP_Brawler_Green, BP_Brawler_Red, BP_Sniper_Green, BP_Sniper_Red };
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
                Tile->GridPosition = FIntPoint(Col, Row);
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

            bool bIsMountain = FMath::RandBool();
            TSubclassOf<AObstacles> Obstacle = bIsMountain ? MountainBlueprint : TreeBlueprint;
            AObstacles* SpawnedObstacle = World->SpawnActor<AObstacles>(Obstacle, Location, FRotator::ZeroRotator);

            if (SpawnedObstacle)
            {
                UE_LOG(LogTemp, Warning, TEXT("✅ Ostacolo spawnato: %s"), *SpawnedObstacle->GetName());
                Tiles[Index]->bHasObstacle = true;
                Tiles[Index]->SetTileOccupied(true);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("❌ Fallito lo spawn di un ostacolo!"));
            }
        }
    }
}
ATile* AGameFeild::GetTileAt(int32 X, int32 Y) const
{
    for (ATile* Tile : Tiles)
    {
        if (Tile && Tile->GridPosition.X == X && Tile->GridPosition.Y == Y)
        {
            return Tile;
        }
    }
    return nullptr;
}
void AGameFeild::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
