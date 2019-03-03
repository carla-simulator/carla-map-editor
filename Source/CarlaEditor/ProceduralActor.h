// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoaderBPFunctionLibrary.h"
#include "ProceduralActor.generated.h"

class UProceduralMeshComponent;

UCLASS()
class CARLAEDITOR_API AProceduralActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProceduralActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostActorCreated() override;

	void PostLoad() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CreateMeshSection(const FMeshInfo& MeshInfo, UMaterial* Material);

private:
	UPROPERTY(Category="Procedural Actor", VisibleAnywhere)
	UProceduralMeshComponent* Mesh;


};
