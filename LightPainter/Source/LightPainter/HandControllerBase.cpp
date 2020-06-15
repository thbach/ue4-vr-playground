// Copyright TB 2020


#include "HandControllerBase.h"
#include "Engine/World.h"


AHandControllerBase::AHandControllerBase()
{

	PrimaryActorTick.bCanEverTick = false;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	MotionController->SetShowDeviceModel(true);
	SetRootComponent(MotionController);
}


void AHandControllerBase::BeginPlay()
{
	Super::BeginPlay();

}

void AHandControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AHandControllerBase::SetHand()
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