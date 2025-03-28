// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFeild.h" // Include il tuo GameFeild



// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
	OccupyingUnit = nullptr;  // Nessun soldato inizialmente

	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	AutoReceiveInput = EAutoReceiveInput::Player0;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetVisibility(true);
	Mesh->bSelectable = true;
	Mesh->SetMobility(EComponentMobility::Static);

	

}
// 🔹 Imposta la posizione della cella nella griglia
void ATile::SetGridPosition(FVector2D NewPosition)
{
	GridPosition = NewPosition;
}
// 🔹 Controlla se la cella è occupata
bool ATile::IsOccupied() const
{
	return OccupyingUnit != nullptr;
}
// 🔹 Assegna un'unità alla cella
void ATile::SetOccupyingUnit(AActor* NewUnit)
{
	OccupyingUnit = NewUnit;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();


	OnClicked.AddDynamic(this, &ATile::OnTileClicked);
	
}
void ATile::OnTileClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("✅ Tile cliccata!"));
	// Chiama funzione su GameFeild per gestire il posizionamento

	AGameFeild* GameField = Cast<AGameFeild>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameFeild::StaticClass()));
	if (GameField && GameField->bGameStarted)
	{
		GameField->HandleTileClicked(this);
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

