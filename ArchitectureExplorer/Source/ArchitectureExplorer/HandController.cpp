// Copyright TB 2020


#include "HandController.h"
#include "MotionControllerComponent.h"

// Sets default values
AHandController::AHandController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);	
	// MotionController->bDisplayDeviceModel = true;
	// MotionController->SetTrackingSource(EControllerHand::Right);	
	
}


void AHandController::SetLeftHand(bool Hand) 
{
	bIsLeftHandSource = Hand;		
}


// Called when the game starts or when spawned
void AHandController::BeginPlay()
{
	Super::BeginPlay();

	SetHand();

}

// Called every frame
void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHandController::SetHand() 
{
	UE_LOG(LogTemp, Warning, TEXT("SetHand"));	
	UMotionControllerComponent* MotionControllerComponent = Cast<UMotionControllerComponent>(GetRootComponent());
	if (MotionControllerComponent)
	{		
		UE_LOG(LogTemp, Warning, TEXT("Motioncontroller"));	
		if (bIsLeftHandSource)
		{
			MotionControllerComponent->SetTrackingSource(EControllerHand::Left);
			UE_LOG(LogTemp, Warning, TEXT("Left"));
		}
		else
		{
			MotionControllerComponent->SetTrackingSource(EControllerHand::Right);
			UE_LOG(LogTemp, Warning, TEXT("Right"));
		}				
	}		
}

