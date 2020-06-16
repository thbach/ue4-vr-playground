// Copyright TB 2020


#include "PaintingGridCard.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/StereoLayerFunctionLibrary.h"


void UPaintingGridCard::SetPaintingName(FString NewPaintingName)
{
    PaintingName = NewPaintingName;
    SlotName->SetText(FText::FromString(PaintingName));

    CardButton->OnClicked.AddDynamic(this, &UPaintingGridCard::CardButtonClicked);
}

void UPaintingGridCard::CardButtonClicked()
{
    UStereoLayerFunctionLibrary::ShowSplashScreen();
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("Canvas"), true, "SlotName=" + PaintingName);
}
