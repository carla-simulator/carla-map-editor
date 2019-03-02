// Fill out your copyright notice in the Description page of Project Settings.

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
