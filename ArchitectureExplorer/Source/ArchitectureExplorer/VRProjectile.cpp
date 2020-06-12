// Copyright TB 2020


#include "VRProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AVRProjectile::AVRProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AVRProjectile::OnHit);	
	SetRootComponent(CollisionComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.6f;

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AVRProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVRProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AVRProjectile::FireInDirection(const FVector& ShootDirection) 
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AVRProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) 
{	
    if (OtherActor != this && OtherActor->ActorHasTag(FName("Para")))
    {
		UE_LOG(LogTemp, Warning, TEXT("Hit2"));
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, Hit.Location, NormalImpulse.Rotation());
		// OtherComponent->Ge
		// OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 1000.0f, Hit.ImpactPoint);
    }	
}
