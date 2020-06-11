// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HandController.h"
#include "VRCharacter.generated.h"


UCLASS()
class ARCHITECTUREEXPLORER_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	// Setup Camera
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot;
	UPROPERTY(VisibleAnywhere)
	class USplineComponent* TeleportPath;	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DestinationMarker;
	
	UPROPERTY(EditDefaultsOnly)	
	TSubclassOf<class AHandController> HandControllerClass;	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AHandController> HandControllerClassRight;
	UPROPERTY(VisibleAnywhere)
	AHandController* LeftController;
	UPROPERTY(VisibleAnywhere)
	AHandController* RightController;	

	UPROPERTY(VisibleAnywhere)
	class UPostProcessComponent* PostProcessComponent;
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* BlinkerMaterialBase;
	UPROPERTY(VisibleAnywhere)
	class UMaterialInstanceDynamic* BlinkerMaterialInstance;
	UPROPERTY(EditAnywhere)
	class UCurveFloat* RadiusVsVelocity;

	UPROPERTY(VisibleAnywherE)
	TArray<class USplineMeshComponent*> TeleportArcMeshPool;	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMesh* TeleportArcMesh;	
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* TeleportArcMaterial;
	
private:

	UPROPERTY(EditAnywhere)
	float MaxTeleportDistance = 1000;
	UPROPERTY(EditAnywhere)
	float TeleportProjectileRadius = 10;	
	UPROPERTY(EditAnywhere)
	float TeleportProjectileSpeed = 800;
	UPROPERTY(EditAnywhere)
	float TeleportFadeTime = 1;
	UPROPERTY(EditAnywhere)
	FVector TeleportProjectionExtent = FVector(100,100,100);



private:

	void MoveForward(float AxisValue); 
	void MoveRight(float AxisValue);
	void GripLeft() { LeftController->Grip(); }
	void ReleaseLeft() { LeftController->Release(); }
	void GripRight() { RightController->Grip(); }
	void ReleaseRight() { RightController->Release(); }
	void BeginTeleport();
	void BeginTeleportHeld();
	void EndTeleport();
	void CameraFade(float FromAlpha, float ToAlpha);

	void UpdateDestinationMarker();
	void HideDestinationMarker();
	bool ShootDestinationMarker(TArray<FVector> &OutPath , FVector &OutLocation);	

	void UpdateBlinkers();
	void UpdateSpline(const TArray<FVector> &Path);
	void DrawTeleportPath(const TArray<FVector> &Path);
	FVector2D GetBlinkerCenter();	

	// State
	bool bTeleportButtonHeld = false;

};
