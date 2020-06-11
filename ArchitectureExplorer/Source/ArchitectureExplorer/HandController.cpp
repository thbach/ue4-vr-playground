// Copyright TB 2020


#include "HandController.h"
#include "MotionControllerComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AHandController::AHandController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);	
	
}

void AHandController::PairController(AHandController* Controller) 
{
	OtherController = Controller;
	OtherController->OtherController = this;
}

void AHandController::SetLeftHand(bool Hand) 
{
	bIsLeftHandSource = Hand;		
}

void AHandController::Grip() 
{	
	if (!bCanClimb) return;		

	if (!bIsClimbing)
	{		
		OtherController->SetIsClimbing(false);		
		bIsClimbing = true;
		ClimbingStartLocation = GetActorLocation();

		ACharacter* Character = Cast<ACharacter>(GetAttachParentActor());
		if (Character)
		{
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		}		
	}	
}

void AHandController::Release() 
{	
	if (bIsClimbing)
	{
		bIsClimbing = false;
		ACharacter* Character = Cast<ACharacter>(GetAttachParentActor());
		if (Character)
		{
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		}
	}	
}

void AHandController::SetIsClimbing(bool IsClimbing)
{
	bIsClimbing = IsClimbing;
}

// Called when the game starts or when spawned
void AHandController::BeginPlay()
{
	Super::BeginPlay();
	
	SetHand();	

	OnActorBeginOverlap.AddDynamic(this, &AHandController::ActorBeginOverLap);
	OnActorEndOverlap.AddDynamic(this, &AHandController::ActorEndOverLap);

}

// Called every frame
void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsClimbing)
	{
		// Offset Camera to body vis versa
		FVector HandControllerDelta = GetActorLocation() - ClimbingStartLocation;			
		GetAttachParentActor()->AddActorWorldOffset(-HandControllerDelta);
		// SetActorLocation(ClimbingStartLocation);		
		// FVector CapsuleOffset = Camera->GetComponentLocation() - GetActorLocation();
		// NewCameraOffset.Z = 0;
		// Actor->AddActorWorldOffset(HandControllerDelta);
		// AddActorWorldOffset(NewCameraOffset);
		// VRRoot->AddWorldOffset(-NewCameraOffset);
	}
	

}

void AHandController::SetHand() 
{	
	MotionControllerComponent = Cast<UMotionControllerComponent>(GetRootComponent());
	if (MotionControllerComponent)
	{		
		if (bIsLeftHandSource)
		{			
			MotionControllerComponent->SetTrackingSource(EControllerHand::Left);					
		}
		else
		{			
			MotionControllerComponent->SetTrackingSource(EControllerHand::Right);			
		}				
	}		
}

void AHandController::ActorBeginOverLap(AActor* OverlappedActor, AActor* OtherActor) 
{
	bool bNewCanClimb = CanClimb();
	if (!bCanClimb && bNewCanClimb)
	{
		APawn* Pawn = Cast<APawn>(GetAttachParentActor());
		if (Pawn)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
			if (PlayerController && CanClimbHapticEffect)
			{
				PlayerController->PlayHapticEffect(CanClimbHapticEffect, MotionControllerComponent->GetTrackingSource());
			}
		}
	}	
	
	bCanClimb = bNewCanClimb;	
}

void AHandController::ActorEndOverLap(AActor* OverlappedActor, AActor* OtherActor) 
{
	bool bNewCanClimb = CanClimb();
}

bool AHandController::CanClimb() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->ActorHasTag(FName("Climbable"))) return true;
	}

	return false;
}

