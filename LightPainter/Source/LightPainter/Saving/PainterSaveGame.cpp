// Copyright TB 2020


#include "PainterSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "../Stroke.h"


UPainterSaveGame* UPainterSaveGame::Create() 
{   
    USaveGame* NewSave = UGameplayStatics::CreateSaveGameObject(StaticClass());
    return Cast<UPainterSaveGame>(NewSave);    
}

bool UPainterSaveGame::Save() 
{
    return UGameplayStatics::SaveGameToSlot(this, TEXT("test"), 0);    
}

UPainterSaveGame* UPainterSaveGame::Load() 
{
    
    USaveGame* NewLoad = UGameplayStatics::LoadGameFromSlot(TEXT("test"), 0);
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
