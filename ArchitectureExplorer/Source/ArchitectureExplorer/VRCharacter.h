// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
	class UStaticMeshComponent* DestinationMarker;

	UPROPERTY(VisibleAnywhere)
	class UPostProcessComponent* PostProcessComponent;
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* BlinkerMaterialBase;
	UPROPERTY(VisibleAnywhere)
	class UMaterialInstanceDynamic* BlinkerMaterialInstance;
	UPROPERTY(EditAnywhere)
	class UCurveFloat* RadiusVsVelocity;

	
	UPROPERTY(EditAnywhere)
	float MaxTeleportDistance = 1000;
	UPROPERTY(EditAnywhere)
	float TeleportFadeTime = 1;
	UPROPERTY(EditAnywhere)
	FVector TeleportProjectionExtent = FVector(100,100,100);



private:

	void MoveForward(float AxisValue); 
	void MoveRight(float AxisValue);
	void BeginTeleport();
	void EndTeleport();
	void CameraFade(float FromAlpha, float ToAlpha);

	bool UpdateDestinationMarker();
	bool FindDestinationMarker(FVector &OutLocation);

	void UpdateBlinkers();
	FVector2D GetBlinkerCenter();

	
	

};
