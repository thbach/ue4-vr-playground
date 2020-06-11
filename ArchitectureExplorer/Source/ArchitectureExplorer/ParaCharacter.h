// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ParaCharacter.generated.h"

UCLASS()
class ARCHITECTUREEXPLORER_API AParaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AParaCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);	


	

};
