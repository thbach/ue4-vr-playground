// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandController.generated.h"

UCLASS()
class ARCHITECTUREEXPLORER_API AHandController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandController();

	void SetLeftHand(bool Hand);

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

	UPROPERTY(EditAnywhere)
	bool bIsLeftHandSource = false;	

	void SetHand();


};
