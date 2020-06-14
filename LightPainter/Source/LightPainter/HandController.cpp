// Copyright TB 2020


#include "HandController.h"
#include "Engine/World.h"


AHandController::AHandController()
{
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	MotionController->SetShowDeviceModel(true);		
	SetRootComponent(MotionController);	
}

void AHandController::TriggerPressed() 
{

	CurrentStroke = GetWorld()->SpawnActor<AStroke>(StrokeClass);
	CurrentStroke->SetActorLocation(GetActorLocation());	
	
}

void AHandController::TriggerReleased() 
{
	CurrentStroke = nullptr;	
}

void AHandController::BeginPlay()
{
	Super::BeginPlay();

	
}

void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentStroke)
	{
		CurrentStroke->Update(GetActorLocation());
	}	

}

void AHandController::SetHand() 
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

