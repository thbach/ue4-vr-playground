// Copyright TB 2020


#include "PaintingGrid.h"
#include "Components/SizeBox.h"
#include "Components/PanelWidget.h"
#include "Components/HorizontalBoxSlot.h"


void UPaintingGrid::AddPainting(int32 PaintingIndex, FString PaintingName)
{
    if (!PaintingGrid) return;
    UPaintingGridCard* NewWidget = CreateWidget<UPaintingGridCard>(GetWorld(), GridCardClass);
    if (!NewWidget) return;

    NewWidget->SetPaintingName(PaintingName);

    USizeBox* CardContainer = Cast<USizeBox>(PaintingGrid->GetChildAt(PaintingIndex));
    if (!CardContainer) return;

    CardContainer->AddChild(NewWidget);

}

void UPaintingGrid::ClearPaintings()
{
    for (int32 i = 0; i < PaintingGrid->GetChildrenCount(); i++)
    {
        USizeBox* CardContainer = Cast<USizeBox>(PaintingGrid->GetChildAt(i));
        if (!CardContainer) continue;
        CardContainer->ClearChildren();
    }
}

void UPaintingGrid::AddPaginationDot(bool Active)
{
    UPaginationDot* Dot = CreateWidget<UPaginationDot>(GetWorld(), PaginationDotClass);
    if (!Dot || !PaginationDots) return;

    Dot->SetActive(Active);
    UHorizontalBoxSlot* HBoxSlot = PaginationDots->AddChildToHorizontalBox(Dot);
    HBoxSlot->SetPadding(FMargin(PaginationDotPadding,0));
}

int32 UPaintingGrid::GetNumberOfSlots() const
{
    return PaintingGrid->GetChildrenCount();
}
