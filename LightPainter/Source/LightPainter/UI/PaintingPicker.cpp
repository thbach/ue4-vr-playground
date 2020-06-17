// Copyright TB 2020


#include "PaintingPicker.h"
#include "PaintingPicker/PaintingGrid.h"
#include "PaintingPicker/ActionBar.h"
#include "../Saving/PainterSaveGame.h"
#include "../Saving/PainterSaveGameIndex.h"
#include "Math/UnrealMathUtility.h"


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

	PaintingGridWidget = Cast<UPaintingGrid>(PaintingGrid->GetUserWidgetObject());

	UActionBar* ActionBarWidget = Cast<UActionBar>(ActionBar->GetUserWidgetObject());
	if (ActionBarWidget)
	{
		ActionBarWidget->SetParentPicker(this);
	}

	UPainterSaveGameIndex* Index = UPainterSaveGameIndex::Load();
	if (!Index) return;

	Refresh();
}

void APaintingPicker::UpdateCurrentPage(int32 Offset)
{
	CurrentPage = FMath::Clamp(CurrentPage + Offset, 0, GetNumberOfPages()-1);
	Refresh();
}

void APaintingPicker::RefreshSlots()
{

	if (!PaintingGridWidget) return;

	PaintingGridWidget->ClearPaintings();

	int32 StartOffset = CurrentPage * PaintingGridWidget->GetNumberOfSlots();
	int32 Counter = 0;
	TArray<FString> SlotNames = UPainterSaveGameIndex::Load()->GetSlotNames();

	for (int32 i = 0; i < PaintingGridWidget->GetNumberOfSlots() && StartOffset + i < SlotNames.Num(); i++)
	{
		FString SlotName = SlotNames[i+StartOffset];
		PaintingGridWidget->AddPainting(i, SlotName);
	}
}

void APaintingPicker::RefreshDots()
{
	if (!PaintingGridWidget) return;

	PaintingGridWidget->ClearDots();

	for (int32 i = 0; i < GetNumberOfPages(); i++)
	{
		PaintingGridWidget->AddPaginationDot(CurrentPage == i);
	}
}

int32 APaintingPicker::GetNumberOfPages() const
{
	if (!PaintingGridWidget) return 0;
	int32 TotalNumberOfSlots = UPainterSaveGameIndex::Load()->GetSlotNames().Num();
	int32 SlotsPerPage = PaintingGridWidget->GetNumberOfSlots();
	return FMath::DivideAndRoundUp(TotalNumberOfSlots, SlotsPerPage);
}

void APaintingPicker::ToggleDeleteMode()
{
	if (!PaintingGridWidget) return;
	PaintingGridWidget->ClearPaintings();
}

void APaintingPicker::AddPainting()
{
	UPainterSaveGame* Painting = UPainterSaveGame::Create();
	// CurrentPage = 0;
	Refresh();
}


