// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EditorGUI.h"
#include "DataExporter.h"
#include "CarlaEditorGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class CARLAEDITOR_API ACarlaEditorGameModeBase : public AGameModeBase {
  GENERATED_BODY()

  public:

  ACarlaEditorGameModeBase(const FObjectInitializer &ObjectInitializer);

  /** Remove the current menu widget and create a new one from the specified
     class, if provided. */
  UFUNCTION(BlueprintCallable, Category = "Carla Editor")
  void ChangeMenuWidget(TSubclassOf<UEditorGUI> NewWidgetClass);

  UFUNCTION(BlueprintCallable, Category = "Carla Editor")
  UEditorGUI *GetEditorGUI() const
  {
    return EditorGUI;
  }

  UFUNCTION(BlueprintCallable, Category = "Carla Editor")
  UDataExporter *GetDataExporter() {
    return DataExporter;
  }

  UFUNCTION(BlueprintCallable, Category = "Carla Editor")
  void ImportOpenDriveFile();

  UFUNCTION(BlueprintCallable, Category = "Carla Editor")
  void ExportOpenDriveFile();

  protected:

  virtual void BeginPlay() override;

  UFUNCTION(BlueprintCallable, Category = "Carla Editor")
  void LoadMeshes();

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Carla Editor")
  TSubclassOf<UEditorGUI> StartingWidgetClass;

  UPROPERTY(VisibleAnywhere, Category = "Carla Editor")
  UEditorGUI *EditorGUI;

  private:

  static TArray<FString> GetAllFilesInDirectory(
      const FString Directory,
      const FString OnlyFilesStartingWith = "",
      const FString OnlyFilesWithExtension = "");

  TMap<FString, UMaterial *> NameMaterialMap;

  UPROPERTY(VisibleAnywhere, Category = "Carla Editor")
  UDataExporter *DataExporter;

  static const FString ImportDirectory;

  static const FString ExportDirectory;
};
