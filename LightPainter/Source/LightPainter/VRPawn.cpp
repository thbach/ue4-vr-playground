// Copyright TB 2020


#include "VRPawn.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UI/PaintingPicker.h"
#include "EngineUtils.h"
#include "Components/InputComponent.h"

// Sets default values
AVRPawn::AVRPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SetRootComponent(VRRoot);
	Camera->SetupAttachment(VRRoot);

}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();


	// Create new map
	// UPainterSaveGame* Painting = UPainterSaveGame::Create();

	if (LeftHandControllerClass)
	{
		LeftHandController = GetWorld()->SpawnActor<AHandControllerBase>(LeftHandControllerClass);
		if (LeftHandController)
		{
			LeftHandController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			LeftHandController->SetLeftHand(true);
			LeftHandController->SetOwner(this);
		}
	}
	if (RightHandControllerClass)
	{
		RightHandController = GetWorld()->SpawnActor<AHandControllerBase>(RightHandControllerClass);
		if (RightHandController)
		{
			RightHandController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			RightHandController->SetLeftHand(false);
			RightHandController->SetOwner(this);
		}
	}
}

void AVRPawn::SetupPlayerInputComponent(UInputComponent*  PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("RightTrigger"), EInputEvent::IE_Pressed, this, &AVRPawn::RightTriggerPressed);
	PlayerInputComponent->BindAction(TEXT("RightTrigger"), EInputEvent::IE_Released, this, &AVRPawn::RightTriggerReleased);
	PlayerInputComponent->BindAxis(TEXT("PaginateRight"), this, &AVRPawn::PaginateRightAxisInput);
}

void AVRPawn::PaginateRightAxisInput(float AxisValue)
{
	if (AxisValue > PaginationAxisThreshold && PreviousAxisValue < PaginationAxisThreshold)
	{
		UpdateCurrentPage(1);
	}

	if (AxisValue < -PaginationAxisThreshold && PreviousAxisValue > -PaginationAxisThreshold)
	{
		UpdateCurrentPage(-1);
	}

	PreviousAxisValue = AxisValue;
}

void AVRPawn::UpdateCurrentPage(int32 Offset)
{
	for (TActorIterator<APaintingPicker> PaintingPickerItr(GetWorld()); PaintingPickerItr; ++PaintingPickerItr )
	{
		PaintingPickerItr->UpdateCurrentPage(Offset);
	}
}