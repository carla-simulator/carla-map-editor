// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "ProceduralActor.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AProceduralActor::AProceduralActor() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));

  RootComponent = Mesh;

  // Enable multi-threaded PhysX cooking.
  Mesh->bUseAsyncCooking = true;
}

// This is called when actor is spawned (at runtime or when you drop it into the
// world in editor)
void AProceduralActor::PostActorCreated() {
  Super::PostActorCreated();
}

// This is called when actor is already in level and map is opened
void AProceduralActor::PostLoad() {
  Super::PostLoad();
}

// Called when the game starts or when spawned
void AProceduralActor::BeginPlay() {
  Super::BeginPlay();
}

// Called every frame
void AProceduralActor::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

void AProceduralActor::CreateMeshSection(const FMeshInfo &MeshInfo, UMaterial *Material) {
  Mesh->CreateMeshSection_LinearColor(0,
      MeshInfo.Vertices,
      MeshInfo.Triangles,
      MeshInfo.Normals,
      MeshInfo.UV0,
      MeshInfo.VertexColors,
      MeshInfo.Tangents,
      false);

  Mesh->SetMaterial(0, Material);

  FRotator Rotation = MeshInfo.WorldTransform.Rotator();
  Rotation.Roll += 180;
  // Change to centimeters
  FVector Scale = MeshInfo.WorldTransform.GetScale3D() * 100;
  SetActorScale3D(FVector(Scale));
  SetActorRotation(Rotation);
  SetActorLocation(MeshInfo.WorldTransform.GetLocation());

  // Disable collision data
  Mesh->ContainsPhysicsTriMeshData(false);
}
