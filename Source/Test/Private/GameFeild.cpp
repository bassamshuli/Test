// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeild.h"
#include "Engine/World.h"
#include "Tile.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AGameFeild::AGameFeild()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Rows = 25;      // Default: 25 righe
	Columns = 25;   // Default: 25 colonne
	CellSize = 37.0f;  // Ogni cella è grande 100 unità

	// 🔹 Trova il Blueprint di Tile e caricalo (Assicurati che il percorso sia corretto!)
	static ConstructorHelpers::FClassFinder<AActor> TileBP(TEXT("/Game/Blueprints/BP_Tile"));
	if (TileBP.Succeeded())
	{
		TileBlueprint = TileBP.Class;
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

// Called when the game starts or when spawned
void AGameFeild::BeginPlay()
{
	Super::BeginPlay();
    GenerateGrid();  // 🔹 Genera la griglia automaticamente
	
}

// Called every frame
void AGameFeild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

