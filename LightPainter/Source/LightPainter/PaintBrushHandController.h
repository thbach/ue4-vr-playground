// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "Stroke.h"
#include "HandControllerBase.h"
#include "PaintBrushHandController.generated.h"

UCLASS()
class LIGHTPAINTER_API APaintBrushHandController : public AHandControllerBase
{
	GENERATED_BODY()

public:

	APaintBrushHandController();

	void TriggerPressed() override;
	void TriggerReleased() override;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

private:
	// Config
	UPROPERTY(EditAnywhere)
	TSubclassOf<AStroke> StrokeClass;


	// Components


	// State
	AStroke* CurrentStroke;




	// functions
};
