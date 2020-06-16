// Copyright TB 2020


#include "PaintingGrid.h"
#include "Components/SizeBox.h"
#include "Components/PanelWidget.h"


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
