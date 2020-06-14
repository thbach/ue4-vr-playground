// Copyright TB 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Saving/PainterSaveGame.h"
#include "Stroke.generated.h"

UCLASS()
class LIGHTPAINTER_API AStroke : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStroke();

	void Update(FVector CursorLocation);

	FStrokeState SerializeToStruct() const;
	static AStroke* SpawnAndDeserializeFromStruct(UWorld* World, const FStrokeState& StrokeState);

private:
	
	FTransform GetNextSegmentTransform(FVector CurrentLocation) const;
	FVector GetNextSegmentScale(FVector CurrentLocation) const;
	FQuat GetNextSegmentRotation(FVector CurrentLocation) const;
	FVector GetNextSegmentLocation(FVector CurrentLocation) const;
	FTransform GetNextJointTransform(FVector CurrentLocation) const;


	// Config


	// Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(Visibleanywhere)
	UInstancedStaticMeshComponent* StrokeMeshes;	

	UPROPERTY(Visibleanywhere)
	UInstancedStaticMeshComponent* JointMeshes;	

	// State
	FVector PreviousCursorLocation;
	TArray<FVector> ControlPoints;

};
