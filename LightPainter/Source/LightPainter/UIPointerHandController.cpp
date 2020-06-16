// Copyright TB 2020


#include "UIPointerHandController.h"
#include "InputCoreTypes.h"

AUIPointerHandController::AUIPointerHandController()
{
	PrimaryActorTick.bCanEverTick = true;

    WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgeInteractionComponent"));
    WidgetInteractionComponent->SetupAttachment(GetRootComponent());

}

void AUIPointerHandController::TriggerPressed()
{
    WidgetInteractionComponent->PressPointerKey(EKeys::LeftMouseButton);
}

void AUIPointerHandController::TriggerReleased()
{
    WidgetInteractionComponent->ReleasePointerKey(EKeys::LeftMouseButton);
}

void AUIPointerHandController::BeginPlay()
{
    Super::BeginPlay();


}

void AUIPointerHandController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

