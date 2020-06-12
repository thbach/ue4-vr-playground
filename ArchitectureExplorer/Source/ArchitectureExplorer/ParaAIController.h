// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ParaAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARCHITECTUREEXPLORER_API AParaAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;	
	
protected:
	virtual void BeginPlay() override;	

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehaviour;
};
