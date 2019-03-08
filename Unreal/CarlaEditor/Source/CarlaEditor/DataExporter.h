// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PugiXML/pugixml.hpp"
#include "DataExporter.generated.h"

USTRUCT(BlueprintType)
struct FEditorTrafficSignData {
  GENERATED_USTRUCT_BODY()

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EditorTrafficSignData")
  FVector Location;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EditorTrafficSignData")
  FRotator Rotation;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EditorTrafficSignData")
  FVector Scale;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EditorTrafficSignData")
  FVector TriggerBoxLocation;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EditorTrafficSignData")
  FRotator TriggerBoxRotation;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EditorTrafficSignData")
  FVector TriggerBoxScale;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EditorTrafficSignData")
  FVector TriggerBoxExtent;
};

USTRUCT(BlueprintType)
struct FEditorSpeedSignData : public FEditorTrafficSignData {
  GENERATED_USTRUCT_BODY()

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EditorSpeedSignData")
  float Speed;
};

USTRUCT(BlueprintType)
struct FEditorTrafficLightData : public FEditorTrafficSignData {
  GENERATED_USTRUCT_BODY()
};

USTRUCT(BlueprintType)
struct FEditorTrafficLightGroupData {
  GENERATED_USTRUCT_BODY()

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EditorTrafficLightGroupData")
  float GreenTime;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EditorTrafficLightGroupData")
  float YellowTime;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EditorTrafficLightGroupData")
  float RedTime;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EditorTrafficLightGroupData")
  TArray<FEditorTrafficLightData> TrafficLights;
};

/**
 *
 */
UCLASS(BlueprintType)
class CARLAEDITOR_API UDataExporter : public UObject {
  GENERATED_BODY()

  private:

  pugi::xml_document MainDoc;

  public:

  UFUNCTION(BlueprintCallable, Category = "DataExporter|File")
  void Load(FString &FilePath);

  UFUNCTION(BlueprintCallable, Category = "DataExporter|CRUD")
  void AddTrafficLightGroups(TArray<FEditorTrafficLightGroupData> TrafficLightGroups);

  UFUNCTION(BlueprintCallable, Category = "DataExporter|CRUD")
  void AddTrafficSigns(TArray<FEditorSpeedSignData> TrafficSigns);

  UFUNCTION(BlueprintCallable, Category = "DataExporter|File")
  void Save(FString &FilePath);

};
