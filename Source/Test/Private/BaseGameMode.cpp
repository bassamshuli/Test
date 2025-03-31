// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

ABaseGameMode::ABaseGameMode() {}

void ABaseGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (World)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
        if (PlayerController)
        {
            for (TActorIterator<ACameraActor> It(World); It; ++It)
            {
                if (It->GetName().Contains("BP_Camera"))
                {
                    PlayerController->SetViewTargetWithBlend(*It, 0.0f);
                    UE_LOG(LogTemp, Warning, TEXT("✅ Camera impostata con successo da BeginPlay!"));
                    break;
                }
            }
        }
    }
}

