// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EditorGUI.generated.h"

/**
 *
 */
UCLASS()
class CARLAEDITOR_API UEditorGUI : public UUserWidget {
  GENERATED_BODY()

  public:

  UFUNCTION(Category = "Editor GUI", BlueprintCallable, BlueprintImplementableEvent,
      meta = (DisplayName = "On Display GUI Request"))
  void DisplayGUI();

  UFUNCTION(Category = "Editor GUI", BlueprintCallable, BlueprintImplementableEvent,
      meta = (DisplayName = "On Display Actor Table Request"))
  void DisplayActorTable();

  UFUNCTION(Category = "Editor GUI", BlueprintCallable, BlueprintImplementableEvent,
      meta = (DisplayName = "On Display Actor Details Request"))
  void DisplayActorDetails(bool bDisplay = true);

  UFUNCTION(Category = "Editor GUI", BlueprintCallable, BlueprintImplementableEvent,
      meta = (DisplayName = "On Select Actor Request"))
  void SelectActor(AActor *Actor);

  UFUNCTION(Category = "Editor GUI", BlueprintCallable, BlueprintImplementableEvent,
      meta = (DisplayName = "On Deselect Actor Request"))
  void DeselectActor(AActor *Actor);

  UFUNCTION(Category = "Editor GUI", BlueprintCallable, BlueprintImplementableEvent,
      meta = (DisplayName = "On Delete Actor Request"))
  void DeleteActor(AActor *Actor);

  UFUNCTION(Category = "Editor GUI", BlueprintCallable, BlueprintImplementableEvent,
      meta = (DisplayName = "On Update Actor Details Request"))
  void UpdateActorDetails();

};
