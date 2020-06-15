// Copyright TB 2020


#include "PaintBrushHandController.h"
#include "Engine/World.h"


APaintBrushHandController::APaintBrushHandController()
{
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	MotionController->SetShowDeviceModel(true);
	SetRootComponent(MotionController);
}

void APaintBrushHandController::TriggerPressed()
{

	CurrentStroke = GetWorld()->SpawnActor<AStroke>(StrokeClass);
	CurrentStroke->SetActorLocation(GetActorLocation());

}

void APaintBrushHandController::TriggerReleased()
{
	CurrentStroke = nullptr;
}

void APaintBrushHandController::BeginPlay()
{
	Super::BeginPlay();


}

void APaintBrushHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentStroke)
	{
		CurrentStroke->Update(GetActorLocation());
	}

}

void APaintBrushHandController::SetHand()
{
	MotionControllerComponent = Cast<UMotionControllerComponent>(GetRootComponent());
	if (MotionControllerComponent)
	{
		if (bIsLeftHand)
		{
			MotionControllerComponent->SetTrackingSource(EControllerHand::Left);
			return;
		}
		MotionControllerComponent->SetTrackingSource(EControllerHand::Right);
	}
}

