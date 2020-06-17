// Copyright TB 2020


#include "PaletteMenuHandController.h"


APaletteMenuHandController::APaletteMenuHandController()
{
    PaletteMenu = CreateDefaultSubobject<UWidgetComponent>(TEXT("PaletteMenu"));
    PaletteMenu->SetupAttachment(GetRootComponent());

}
