// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroGameMode.h"
#include "Blueprint/UserWidget.h"


AIntroGameMode::AIntroGameMode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/BluePrints/BP_Widgets/WBP_StartMenu")); // <-- path corretto!
    if (WidgetClass.Succeeded())
    {
        StartMenuWidgetClass = WidgetClass.Class;
    }
}

void AIntroGameMode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("IntroGameMode: BeginPlay() called"));

    if (StartMenuWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("IntroGameMode: Creating Widget..."));

        UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), StartMenuWidgetClass);
        if (Widget)
        {
            Widget->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("IntroGameMode: Widget Added!"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("IntroGameMode: Widget creation failed"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("IntroGameMode: StartMenuWidgetClass is NULL"));
    }
}


