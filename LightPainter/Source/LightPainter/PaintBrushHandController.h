// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionControllerComponent.h"
#include "Stroke.h"
#include "HandControllerBase.h"
#include "PaintBrushHandController.generated.h"

UCLASS()
class LIGHTPAINTER_API APaintBrushHandController : public AHandControllerBase
{
	GENERATED_BODY()

public:

	APaintBrushHandController();

	void SetLeftHand(bool Hand) { bIsLeftHand = Hand; SetHand(); }
	void TriggerPressed();
	void TriggerReleased();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

private:
	// Config
	UPROPERTY(EditAnywhere)
	TSubclassOf<AStroke> StrokeClass;
	UPROPERTY(EditAnywhere)
	bool bIsLeftHand = false;

	// Components
	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* MotionController;
	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* MotionControllerComponent;

	// State
	AStroke* CurrentStroke;




	// functions
	void SetHand();
};
