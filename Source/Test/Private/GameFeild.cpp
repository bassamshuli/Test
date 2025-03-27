// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeild.h"
#include "Engine/World.h"
#include "Tile.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AGameFeild::AGameFeild()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    GenerateGrid();  // 🔹 Genera la griglia automaticamente

	Rows = 25;      // Default: 25 righe
	Columns = 25;   // Default: 25 colonne
	CellSize = 37.0f;  // Ogni cella è grande 100 unità

	// 🔹 Trova il Blueprint di Tile e caricalo (Assicurati che il percorso sia corretto!)
	static ConstructorHelpers::FClassFinder<AActor> TileBP(TEXT("/Game/Blueprints/BP_Tile"));
	if (TileBP.Succeeded())
	{
		TileBlueprint = TileBP.Class;
	}
    static ConstructorHelpers::FClassFinder<AObstacles> MountainBP(TEXT("/Game/Blueprints/BP_Mountain"));
    if (MountainBP.Succeeded())
    {
        MountainBlueprint = MountainBP.Class;
    }

    static ConstructorHelpers::FClassFinder<AObstacles> TreeBP(TEXT("/Game/Blueprints/BP_Tree"));
    if (TreeBP.Succeeded())
    {
        TreeBlueprint = TreeBP.Class;
    }
    static ConstructorHelpers::FClassFinder<AObstacles> ObstacleBP(TEXT("/Game/Blueprints/BP_Tree"));
    if (ObstacleBP.Succeeded())
    {
        ObstacleToSpawn = ObstacleBP.Class;
    }


}

void AGameFeild::GenerateGrid()
{
    UE_LOG(LogTemp, Warning, TEXT("✅ GenerateGrid() è stata chiamata!"));

    UWorld* World = GetWorld();
    if (!World || !TileBlueprint)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ERRORE: World o TileBlueprint è NULL!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("GenerateGrid: Inizio generazione griglia..."));

    Tiles.Empty();  // 🔹 Svuota l'array prima di creare nuovi Tile

    float Spacing = 5.0f; // 🔹 Aggiunge spazio tra i Tile
    float AdjustedCellSize = CellSize + Spacing; // 🔹 CellSize con margine incluso

    float StartX = -((Columns - 1) * AdjustedCellSize) / 2.0f;
    float StartY = -((Rows - 1) * AdjustedCellSize) / 2.0f;

    int32 TileCount = 0;  // 🔹 Contatore dei Tile generati

    for (int32 Row = 0; Row < Rows; Row++)
    {
        for (int32 Col = 0; Col < Columns; Col++)
        {
            // 🔹 Posizionamento con spaziatura tra i Tile
            FVector CellLocation = FVector(StartX + (Col * AdjustedCellSize), StartY + (Row * AdjustedCellSize), 0);
            FRotator Rotation = FRotator::ZeroRotator;
            FActorSpawnParameters SpawnParams;

            AActor* NewTile = World->SpawnActor<AActor>(TileBlueprint, CellLocation, Rotation, SpawnParams);

            if (NewTile)
            {
                UE_LOG(LogTemp, Warning, TEXT("✅ Tile %d creato in posizione (%d, %d) -> Posizione reale: X=%f, Y=%f"),
                    TileCount, Row, Col, CellLocation.X, CellLocation.Y);

                ATile* TileInstance = Cast<ATile>(NewTile);
                if (TileInstance)
                {
                    Tiles.Add(TileInstance);
                }

                TileCount++;  // 🔹 Incrementa il contatore
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ GenerateGrid: Griglia generata con %d Tile!"), TileCount);
}
void AGameFeild::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    GenerateGrid(); // Questa chiamata mostra la griglia appena carichi la mappa
}


void AGameFeild::GenerateObstacles()
{
    UWorld* World = GetWorld();
    if (!World || (!MountainBlueprint && !TreeBlueprint)) return;

    int32 NumObstacles = FMath::RoundToInt((Rows * Columns) * 0.1f);
    TSet<int32> UsedIndices;

    for (int32 i = 0; i < NumObstacles; i++)
    {
        int32 RandomIndex;
        do {
            RandomIndex = FMath::RandRange(0, Tiles.Num() - 1);
        } while (UsedIndices.Contains(RandomIndex));

        UsedIndices.Add(RandomIndex);

        FVector SpawnLocation = Tiles[RandomIndex]->GetActorLocation() + FVector(0, 0, 10);

        // Usa direttamente la variabile membro ObstacleToSpawn
        ObstacleToSpawn = FMath::RandBool() ? MountainBlueprint : TreeBlueprint;

        if (!ObstacleToSpawn)
        {
            UE_LOG(LogTemp, Error, TEXT("❌ ERRORE: ObstacleToSpawn è NULL!"));
            continue;
        }

        AObstacles* NewObstacle = World->SpawnActor<AObstacles>(ObstacleToSpawn, SpawnLocation, FRotator::ZeroRotator);

        if (NewObstacle)
        {
            UE_LOG(LogTemp, Warning, TEXT("✅ Ostacolo posizionato su Tile %d."), RandomIndex);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ ERRORE: Ostacolo NON spawnato su Tile %d."), RandomIndex);
        }
    }
}


// Called when the game starts or when spawned
void AGameFeild::BeginPlay()
{
	Super::BeginPlay();

    GenerateObstacles();

    if (GameWidgetClass)
    {
        GameWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), GameWidgetClass);
        if (GameWidgetInstance)
        {
            GameWidgetInstance->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("✅ Widget WBP_Game aggiunto al viewport!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GameWidgetClass NON assegnato!"));
    }
    



	
}


// Called every frame
void AGameFeild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

