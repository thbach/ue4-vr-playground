// Copyright TB 2020


#include "PainterSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Misc/Guid.h"
#include "PainterSaveGameIndex.h"
#include "../Stroke.h"


UPainterSaveGame* UPainterSaveGame::Create()
{
    UPainterSaveGame* NewSave = Cast<UPainterSaveGame>(UGameplayStatics::CreateSaveGameObject(StaticClass()));
    NewSave->SlotName = FGuid::NewGuid().ToString();
    if (!NewSave->Save()) return nullptr;

    UPainterSaveGameIndex* Index = UPainterSaveGameIndex::Load();
    Index->AddSaveGame(NewSave);
    Index->Save();
    return NewSave;
}

bool UPainterSaveGame::Save()
{
    return UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}

UPainterSaveGame* UPainterSaveGame::Load(FString SlotName)
{
    USaveGame* NewLoad = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
    return Cast<UPainterSaveGame>(NewLoad);
}

void UPainterSaveGame::SerializeFromWorld(UWorld* World)
{
    Strokes.Empty();
    for (TActorIterator<AStroke> StrokeItr(World); StrokeItr; ++StrokeItr)
    {
        Strokes.Add(StrokeItr->SerializeToStruct());
    }
}

void UPainterSaveGame::DeserializeToWorld(UWorld* World)
{
    ClearWorld(World);
    for (FStrokeState StrokeState : Strokes)
    {
        AStroke::SpawnAndDeserializeFromStruct(World, StrokeState);
    }
}

void UPainterSaveGame::ClearWorld(UWorld* World)
{
    for (TActorIterator<AStroke> StrokeItr(World); StrokeItr; ++StrokeItr)
    {
        StrokeItr->Destroy();
    }
}
