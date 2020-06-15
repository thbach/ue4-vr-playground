// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandControllerBase.generated.h"

UCLASS()
class LIGHTPAINTER_API AHandControllerBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHandControllerBase();

	void SetLeftHand(bool Hand) { bIsLeftHand = Hand; SetHand(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Config
	UPROPERTY(EditAnywhere)
	bool bIsLeftHand = false;

	// Components
	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* MotionController;
	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* MotionControllerComponent;





	// functions
	void SetHand();

};
