// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HandControllerBase.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "VRPawn.generated.h"

UCLASS()
class LIGHTPAINTER_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	AVRPawn();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent*  PlayerInputComponent) override;


private:
	// Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* VRRoot;
	UPROPERTY(Visibleanywhere)
	UCameraComponent* Camera;

private:
	// Config
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHandControllerBase> HandControllerClass;

	// Children
	UPROPERTY(VisibleAnywhere)
	AHandControllerBase* LeftHandController;
	UPROPERTY(VisibleAnywhere)
	AHandControllerBase* RightHandController;

	// functions
	void RightTriggerPressed() { if (RightHandController) RightHandController->TriggerPressed(); }
	void RightTriggerReleased() { if (RightHandController) RightHandController->TriggerReleased(); }

	void Save();

	// State

};
