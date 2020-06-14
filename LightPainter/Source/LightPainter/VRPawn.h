// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HandController.h"
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
	TSubclassOf<AHandController> HandControllerClass;

	// Children
	UPROPERTY(VisibleAnywhere)
	AHandController* LeftHandController;
	UPROPERTY(VisibleAnywhere)	
	AHandController* RightHandController;
	
	// functions
	void RightTriggerPressed() { if (RightHandController) RightHandController->TriggerPressed(); }
	void RightTriggerReleased() { if (RightHandController) RightHandController->TriggerReleased(); }

	void Save();
	void Load();

};
