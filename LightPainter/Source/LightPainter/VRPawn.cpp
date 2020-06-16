// Copyright TB 2020


#include "VRPawn.h"
#include "Engine/World.h"
#include "PaintingGameMode.h"
#include "Saving/PainterSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/StereoLayerFunctionLibrary.h"
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

	if (HandControllerClass)
	{
		LeftHandController = GetWorld()->SpawnActor<AHandControllerBase>(HandControllerClass);
		if (LeftHandController)
		{
			LeftHandController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			LeftHandController->SetLeftHand(true);
			LeftHandController->SetOwner(this);
		}
		RightHandController = GetWorld()->SpawnActor<AHandControllerBase>(HandControllerClass);
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
	PlayerInputComponent->BindAction(TEXT("Save"), EInputEvent::IE_Released, this, &AVRPawn::Save);

}

void AVRPawn::Save()
{
	auto GameMode = Cast<APaintingGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;
	GameMode->Save();
    UStereoLayerFunctionLibrary::ShowSplashScreen();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}

