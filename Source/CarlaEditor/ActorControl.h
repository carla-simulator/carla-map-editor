// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorControl.generated.h"

UCLASS()
class CARLAEDITOR_API AActorControl : public AActor {
  GENERATED_BODY()

  public:

  // Sets default values for this actor's properties
  AActorControl();

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(Category = "Actor Control", BlueprintCallable, BlueprintImplementableEvent,
      meta = (DisplayName = "On Deselect Request"))
  void Deselect();

  UFUNCTION(Category = "Actor Control", BlueprintCallable)
  FVector GetDirection() const {
    return Direction;
  }

  UFUNCTION(Category = "Actor Control", BlueprintCallable)
  void SetDirection(FVector InDirection);

  UFUNCTION(Category = "Actor Control", BlueprintCallable)
  bool IsHeld() const {
    return bIsHeld;
  }

  UFUNCTION(Category = "Actor Control", BlueprintCallable)
  void SetIsHeld(bool bInIsHeld);

  protected:

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  UPROPERTY(Category = "Actor Control", BlueprintReadWrite, VisibleAnywhere)
  FVector Direction = FVector(0, 0, 0);

  UPROPERTY(Category = "Actor Control", BlueprintReadWrite, VisibleAnywhere)
  bool bIsHeld = false;

  public:

};
