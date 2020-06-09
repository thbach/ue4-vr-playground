// Copyright TB 2020


#include "VRCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "NavigationSystem.h"
#include "TimerManager.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/Vector.h"
#include "Curves/CurveFloat.h"

#define OUT
// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	DestinationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DesitinationMarker"));
	DestinationMarker->SetupAttachment(GetRootComponent());

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	DestinationMarker->SetVisibility(false);	

	if (BlinkerMaterialBase)
	{
		BlinkerMaterialInstance = UMaterialInstanceDynamic::Create(BlinkerMaterialBase, this);
		PostProcessComponent->AddOrUpdateBlendable(BlinkerMaterialInstance);
	}	
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Offset Camera to body vis versa
	FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = 0;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);

	UpdateDestinationMarker();
}

bool AVRCharacter::UpdateDestinationMarker() 
{
	FVector OutLocation;
	
	if (FindDestinationMarker(OutLocation))
	{
		DestinationMarker->SetWorldLocation(OutLocation);
		DestinationMarker->SetVisibility(true);
	}
	else
	{
		DestinationMarker->SetVisibility(false);
	}
	
	return true;
}

bool AVRCharacter::FindDestinationMarker(FVector &OutLocation) 
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Camera->GetForwardVector() * MaxTeleportDistance;

	FHitResult HitResult;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(OUT HitResult, OUT Start, OUT End, ECC_Visibility);

	if(!bHit) return false;

	FNavLocation NavLocation;
	const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	bool bOnNavMesh = NavSystem->ProjectPointToNavigation(HitResult.Location, NavLocation, TeleportProjectionExtent);	

	if (!bOnNavMesh) return false;
	OutLocation = NavLocation.Location;

	return true;
}

void AVRCharacter::UpdateBlinkers() 
{
	if (!RadiusVsVelocity) return;
	float Radius = RadiusVsVelocity->GetFloatValue(GetVelocity().Size());
	BlinkerMaterialInstance->SetScalarParameterValue(TEXT("RadialRadius"), Radius);

	FVector2D CenterPosition = GetBlinkerCenter();
	BlinkerMaterialInstance->SetVectorParameterValue(TEXT("CenterPosition"), FLinearColor(CenterPosition.X, CenterPosition.Y, 0));
}

FVector2D AVRCharacter::GetBlinkerCenter() 
{
	FVector MovementDirection = GetVelocity().GetSafeNormal();
	if (MovementDirection.IsNearlyZero()) return FVector2D(0.5, 0.5);

	FVector WorldStationaryLocation;
	if (FVector::DotProduct(Camera->GetForwardVector(), MovementDirection) > 0)
	{
		WorldStationaryLocation = Camera->GetComponentLocation() + MovementDirection * 1000;
	}
	else
	{
		WorldStationaryLocation = Camera->GetComponentLocation() - MovementDirection * 1000;		
	}
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return FVector2D(0.5, 0.5);

	FVector2D ScreenStationaryLocation;
	PC->ProjectWorldLocationToScreen(WorldStationaryLocation, ScreenStationaryLocation);

	int32 SizeX, SizeY;
	PC->GetViewportSize(SizeX, SizeY);
	ScreenStationaryLocation.X /= SizeX;
	ScreenStationaryLocation.Y /= SizeY;

	return FVector2D(ScreenStationaryLocation);
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AVRCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AVRCharacter::MoveRight);
	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Released, this, &AVRCharacter::BeginTeleport);
}


void AVRCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(Camera->GetForwardVector() * AxisValue);
	UpdateBlinkers();
}

void AVRCharacter::MoveRight(float AxisValue) 
{
	AddMovementInput(Camera->GetRightVector() * AxisValue);
	UpdateBlinkers();
}

void AVRCharacter::BeginTeleport() 
{
	CameraFade(0,1);

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &AVRCharacter::EndTeleport, TeleportFadeTime);	
}

void AVRCharacter::EndTeleport() 
{
	FVector Location = DestinationMarker->GetComponentLocation();
	Location.Z = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	SetActorLocation(Location);
	CameraFade(1,0);
}

void AVRCharacter::CameraFade(float FromAlpha, float ToAlpha) 
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->PlayerCameraManager->StartCameraFade(FromAlpha, ToAlpha, TeleportFadeTime, FColor::Black, false, true);
	}		
}



