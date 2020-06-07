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


	void MoveForward(float AxisValue); 
	void MoveRight(float AxisValue);

};