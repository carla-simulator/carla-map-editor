// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.



#include "ActorControl.h"


// Sets default values
AActorControl::AActorControl()
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AActorControl::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AActorControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActorControl::SetDirection(FVector InDirection)
{
	Direction = InDirection;
}

void AActorControl::SetIsHeld(bool bInIsHeld)
{
	bIsHeld = bInIsHeld;
}
