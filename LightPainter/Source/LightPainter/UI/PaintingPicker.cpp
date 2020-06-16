// Copyright TB 2020


#include "PaintingPicker.h"
#include "PaintingPicker/PaintingGrid.h"
#include "PaintingPicker/ActionBar.h"
#include "../Saving/PainterSaveGame.h"
#include "../Saving/PainterSaveGameIndex.h"


APaintingPicker::APaintingPicker()
{

	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PaintingGrid = CreateDefaultSubobject<UWidgetComponent>(TEXT("PaintingGrid"));
	PaintingGrid->SetupAttachment(Root);
	PaintingGrid->SetDrawSize(FVector2D(1000, 1000));
	PaintingGrid->SetWorldLocation(FVector(249,0,-26));
	PaintingGrid->SetWorldRotation(FRotator(0,180,0));
	PaintingGrid->SetWorldScale3D(FVector(0.25));

	ActionBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("ActionBar"));
	ActionBar->SetupAttachment(Root);
	ActionBar->SetDrawSize(FVector2D(208, 96));
	ActionBar->SetWorldLocation(FVector(198,0,-114));
	ActionBar->SetWorldRotation(FRotator(0,180,0));
	ActionBar->SetWorldScale3D(FVector(0.225));

}

void APaintingPicker::BeginPlay()
{
	Super::BeginPlay();

	UActionBar* ActionBarWidget = Cast<UActionBar>(ActionBar->GetUserWidgetObject());
	if (ActionBarWidget)
	{
		ActionBarWidget->SetParentPicker(this);
	}

	UPainterSaveGameIndex* Index = UPainterSaveGameIndex::Load();
	if (!Index) return;

	RefreshSlots();
}

void APaintingPicker::RefreshSlots()
{
	UPaintingGrid* PaintingGridWidget = Cast<UPaintingGrid>(PaintingGrid->GetUserWidgetObject());
	if (!PaintingGridWidget) return;

	PaintingGridWidget->ClearPaintings();

	int32 Counter = 0;
	for (FString SlotName : UPainterSaveGameIndex::Load()->GetSlotNames())
	{
		PaintingGridWidget->AddPainting(Counter, SlotName);
		++Counter;
	}
}

void APaintingPicker::ToggleDeleteMode()
{
    UE_LOG(LogTemp, Warning, TEXT("Delete clicked"));
	UPaintingGrid* PaintingGridWidget = Cast<UPaintingGrid>(PaintingGrid->GetUserWidgetObject());
	if (!PaintingGridWidget) return;

	PaintingGridWidget->ClearPaintings();
}

void APaintingPicker::AddPainting()
{
	UPainterSaveGame* Painting = UPainterSaveGame::Create();
	RefreshSlots();
}

