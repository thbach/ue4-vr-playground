// Copyright TB 2020


#include "PaintingGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/StereoLayerFunctionLibrary.h"
#include "Engine/World.h"
#include "Saving/PainterSaveGame.h"



void APaintingGameMode::InitGame(const FString &MapName, const FString &Options, FString &ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    SlotName = UGameplayStatics::ParseOption(Options, "SlotName");
}



void APaintingGameMode::BeginPlay()
{
    Super::BeginPlay();

    UStereoLayerFunctionLibrary::HideSplashScreen();

    Load();
}

void APaintingGameMode::Save()
{
	UPainterSaveGame* Painting = UPainterSaveGame::Load(SlotName);
	if (Painting)
	{
		Painting->SerializeFromWorld(GetWorld());
		Painting->Save();
	}
}

void APaintingGameMode::Load()
{
	UPainterSaveGame* Painting = UPainterSaveGame::Load(SlotName);
	if (Painting)
	{
		Painting->DeserializeToWorld(GetWorld());

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameSlot not found %s"), *SlotName);
	}
}

void APaintingGameMode::SaveAndQuit()
{
	Save();
    UStereoLayerFunctionLibrary::ShowSplashScreen();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}