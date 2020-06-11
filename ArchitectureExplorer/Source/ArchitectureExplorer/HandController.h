// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputCoreTypes.h"
#include "HandController.generated.h"

UCLASS()
class ARCHITECTUREEXPLORER_API AHandController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandController();

	void PairController(AHandController* Controller);
	void SetLeftHand(bool Hand);
	void Grip();
	void Release();
	void SetIsClimbing(bool IsClimbing);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	// Default sub object
	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent* MotionController;
	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent* MotionControllerComponent;

	// Params
	UPROPERTY(EditAnywhere)
	bool bIsLeftHandSource = false;	

	UPROPERTY(EditDefaultsOnly)
	class UHapticFeedbackEffect_Base* CanClimbHapticEffect;

	// Callbacks
	UFUNCTION()
	void ActorBeginOverLap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void ActorEndOverLap(AActor* OverlappedActor, AActor* OtherActor);

	// Helpers

	bool CanClimb() const;
	void SetHand();	

	// State
	bool bCanClimb = false;
	bool bIsClimbing = false;
	FVector ClimbingStartLocation;

	AHandController* OtherController;

};
