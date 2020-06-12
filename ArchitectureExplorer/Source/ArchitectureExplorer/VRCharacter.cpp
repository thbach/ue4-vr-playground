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
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "MotionControllerComponent.h"
#include "Engine/World.h"
#include "VRCharacter.h"
#include "VRProjectile.h"
#include "HandController.h"


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

	TeleportPath = CreateDefaultSubobject<USplineComponent>(TEXT("TeleportPath"));
	TeleportPath->SetupAttachment(VRRoot);

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
		// Turn on post process
		// PostProcessComponent->AddOrUpdateBlendable(BlinkerMaterialInstance);
	}	

	// Setup Controller Actors
	LeftController = GetWorld()->SpawnActor<AHandController>(HandControllerClass);
	if (LeftController != nullptr)
	{
		LeftController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);		
		LeftController->SetLeftHand(true);			
		LeftController->SetOwner(this);		
	}
	RightController = GetWorld()->SpawnActor<AHandController>(HandControllerClassRight);
	if (RightController != nullptr)
	{
		RightController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);				
		RightController->SetLeftHand(false);			
		RightController->SetOwner(this);		
	}	
	LeftController->PairController(RightController);
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

	if (bTeleportButtonHeld) UpdateDestinationMarker();	

}

void AVRCharacter::UpdateDestinationMarker() 
{
	TArray<FVector> Path;	
	FVector Location;
	if (ShootDestinationMarker(OUT Path, OUT Location))
	{
		DestinationMarker->SetWorldLocation(Location);
		DestinationMarker->SetVisibility(true);
		DrawTeleportPath(OUT Path);
	}
	else
	{
		DestinationMarker->SetVisibility(false);
		TArray<FVector> EmptyPath;
		DrawTeleportPath(EmptyPath);
	}
	
	// return true;
}

void AVRCharacter::HideDestinationMarker() 
{
	bTeleportButtonHeld = false;	
	DestinationMarker->SetVisibility(false);	
	for (USplineMeshComponent* SplineMesh : TeleportArcMeshPool)
	{
		SplineMesh->SetVisibility(false);
	}	
}

bool AVRCharacter::ShootDestinationMarker(TArray<FVector> &OutPath , FVector &OutLocation)
{
	FVector Start = RightController->GetActorLocation();
	FVector Look = RightController->GetActorForwardVector();
	Look.RotateAngleAxis(45, RightController->GetActorRightVector());

	FPredictProjectilePathParams PredictParams = FPredictProjectilePathParams
	(
		TeleportProjectileRadius,
		Start,
		Look * TeleportProjectileSpeed,
		2,
		ECC_Visibility,
		this
	);

	// PredictParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;

	FPredictProjectilePathResult PredictResult;
	bool bHit = UGameplayStatics::PredictProjectilePath(this, PredictParams, OUT PredictResult);

	if(!bHit) return false;

	for (FPredictProjectilePathPointData PointData : PredictResult.PathData)
	{
		OutPath.Add(PointData.Location);
	}
	
	FNavLocation NavLocation;
	const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	bool bOnNavMesh = NavSystem->ProjectPointToNavigation(PredictResult.HitResult.Location, NavLocation, TeleportProjectionExtent);	

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

void AVRCharacter::DrawTeleportPath(const TArray<FVector> &Path) 
{
	UpdateSpline(Path);

	for (USplineMeshComponent* SplineMesh : TeleportArcMeshPool)
	{
		SplineMesh->SetVisibility(false);
	}
	

	int32 SegmentNum = Path.Num() - 1;
	for (int32 i = 0; i < SegmentNum; i++)
	{
		if (TeleportArcMeshPool.Num() <= i)
		{
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
			SplineMesh->SetMobility(EComponentMobility::Movable);
			SplineMesh->AttachToComponent(TeleportPath, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMesh->SetStaticMesh(TeleportArcMesh);
			SplineMesh->SetMaterial(0, TeleportArcMaterial);
			SplineMesh->RegisterComponent();			

			TeleportArcMeshPool.Add(SplineMesh);
		}

		FVector StartPos, StartTangent, EndPos, EndTangent;
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i, OUT StartPos, OUT StartTangent);
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i + 1, OUT EndPos, OUT EndTangent);

		TeleportArcMeshPool[i]->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
		TeleportArcMeshPool[i]->SetVisibility(true);
	}	
}

void AVRCharacter::UpdateSpline(const TArray<FVector> &Path) 
{
	TeleportPath->ClearSplinePoints(false);
	for (int32 i = 0; i < Path.Num(); i++)
	{
		FVector LocalPosition = TeleportPath->GetComponentTransform().InverseTransformPosition(Path[i]);
		FSplinePoint Point = FSplinePoint(i, LocalPosition, ESplinePointType::Curve);
		TeleportPath->AddPoint(Point, false);				
	}
	TeleportPath->UpdateSpline();

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
	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Repeat, this, &AVRCharacter::BeginTeleportHeld);
	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Released, this, &AVRCharacter::BeginTeleport);	
	PlayerInputComponent->BindAction(TEXT("GripLeft"), IE_Pressed, this, &AVRCharacter::GripLeft);	
	PlayerInputComponent->BindAction(TEXT("GripRight"), IE_Pressed, this, &AVRCharacter::GripRight);	
	PlayerInputComponent->BindAction(TEXT("GripLeft"), IE_Released, this, &AVRCharacter::ReleaseLeft);	
	PlayerInputComponent->BindAction(TEXT("GripRight"), IE_Released, this, &AVRCharacter::ReleaseRight);	
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AVRCharacter::Fire);				
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

void AVRCharacter::BeginTeleportHeld() 
{
	bTeleportButtonHeld = true;
}

void AVRCharacter::EndTeleport() 
{	
	HideDestinationMarker();
	FVector Location = DestinationMarker->GetComponentLocation();
	Location += GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * GetActorUpVector();
	// Location.Z = Location;
	SetActorLocation(Location);
	CameraFade(1,0);
}

void AVRCharacter::Fire() 
{
	FVector MuzzleLocation = RightController->GetActorLocation();
	FRotator MuzzleRotation = RightController->GetActorRotation();
	// Skew the aim to be slightly upwards.
	MuzzleRotation.Pitch += 10.0f;
	MuzzleRotation.Yaw += 20.0f;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;	
	AVRProjectile* Projectile = GetWorld()->SpawnActor<AVRProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	if (Projectile)
	{
		Projectile->FireInDirection(MuzzleRotation.Vector());				
	}   

}


void AVRCharacter::CameraFade(float FromAlpha, float ToAlpha) 
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->PlayerCameraManager->StartCameraFade(FromAlpha, ToAlpha, TeleportFadeTime, FColor::Black, false, true);
	}		
}



