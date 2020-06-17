// Copyright TB 2020


#include "WidgetTouchingComponent.h"
#include "../PaintingGameMode.h"


void UWidgetTouchingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (IsOverInteractableWidget() && !bIsClicked)
    {
        UE_LOG(LogTemp, Warning, TEXT("TOUCHING"));
        PressPointerKey(EKeys::LeftMouseButton);
        bIsClicked = true;
    }

    if (!IsOverInteractableWidget() && bIsClicked)
    {
        ReleasePointerKey(EKeys::LeftMouseButton);
        UE_LOG(LogTemp, Warning, TEXT("NOT"));
        bIsClicked = false;
    }

}
