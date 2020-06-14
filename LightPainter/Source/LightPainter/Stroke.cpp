// Copyright TB 2020


#include "Stroke.h"
#include "Math/Quat.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/World.h"


// Sets default values
AStroke::AStroke()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	StrokeMeshes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStrokeMesh"));	
	StrokeMeshes->SetupAttachment(Root);
	JointMeshes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedJointMesh"));	
	JointMeshes->SetupAttachment(Root);	

}

void AStroke::Update(FVector CursorLocation) 
{
	ControlPoints.Add(CursorLocation);

	if (PreviousCursorLocation.IsNearlyZero()) 
	{
		PreviousCursorLocation = CursorLocation; 
		JointMeshes->AddInstance(GetNextJointTransform(CursorLocation));		
		return;	
	}
	
	StrokeMeshes->AddInstance(GetNextSegmentTransform(CursorLocation));		
	JointMeshes->AddInstance(GetNextJointTransform(CursorLocation));
	PreviousCursorLocation = CursorLocation;
}

FStrokeState AStroke::SerializeToStruct() const
{
	FStrokeState StrokeState;
	StrokeState.Class = GetClass();
	StrokeState.ControlPoints = ControlPoints;
	
	return StrokeState;
}

AStroke* AStroke::SpawnAndDeserializeFromStruct(UWorld* World, const FStrokeState& StrokeState) 
{
	AStroke* Stroke = World->SpawnActor<AStroke>(StrokeState.Class);	
	for (FVector ControlPoint : StrokeState.ControlPoints)
	{
		Stroke->Update(ControlPoint);
	}		
	return Stroke;
}

FTransform AStroke::GetNextSegmentTransform(FVector CurrentLocation) const
{
	FTransform SegmentTransform;

	SegmentTransform.SetScale3D(GetNextSegmentScale(CurrentLocation));
	SegmentTransform.SetRotation(GetNextSegmentRotation(CurrentLocation));
	SegmentTransform.SetLocation(GetNextSegmentLocation(CurrentLocation));

	
	return SegmentTransform;
	
}

FVector AStroke::GetNextSegmentScale(FVector CurrentLocation) const
{
	FVector Segment = CurrentLocation-PreviousCursorLocation;
	
	return FVector(Segment.Size(), 1, 1);
}

FQuat AStroke::GetNextSegmentRotation(FVector CurrentLocation) const
{
	FVector Segment = CurrentLocation-PreviousCursorLocation;
	FVector SegmentNormal = Segment.GetSafeNormal();

	return FQuat::FindBetweenNormals(FVector::ForwardVector, SegmentNormal);
}

FVector AStroke::GetNextSegmentLocation(FVector CurrentLocation) const
{
	return GetTransform().InverseTransformPosition(PreviousCursorLocation);
}

FTransform AStroke::GetNextJointTransform(FVector CurrentLocation) const
{
	FTransform JointTransform;
	JointTransform.SetLocation(GetTransform().InverseTransformPosition(CurrentLocation));	
	return JointTransform;
}
