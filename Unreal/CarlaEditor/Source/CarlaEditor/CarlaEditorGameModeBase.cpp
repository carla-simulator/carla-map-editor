// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "CarlaEditorGameModeBase.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/LocalTimestampDirectoryVisitor.h"
#include "LoaderBPFunctionLibrary.h"
#include "ProceduralActor.h"

ACarlaEditorGameModeBase::ACarlaEditorGameModeBase(const FObjectInitializer &ObjectInitializer) : Super(
                                                                                                      ObjectInitializer)
{
  static ConstructorHelpers::FObjectFinder<UMaterial> RoadNode(TEXT(
      "Material'/Game/Carla/Static/GenericMaterials/Masters/LowComplexity/M_Road1.M_Road1'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> RoadNodeAux(TEXT(
      "Material'/Game/Carla/Static/GenericMaterials/LaneMarking/M_MarkingLane_Y.M_MarkingLane_Y'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> MarkingNode(TEXT(
      "Material'/Game/Carla/Static/GenericMaterials/LaneMarking/M_MarkingLane_W.M_MarkingLane_W'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> TerrainNode(TEXT(
      "Material'/Game/Carla/Static/GenericMaterials/Grass/M_Grass01.M_Grass01'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> ConcreteNode(TEXT(
      "Material'/Game/Carla/Static/GenericMaterials/Concrete/M_Concrete_04'"));

  if (RoadNode.Succeeded()) {
    NameMaterialMap.Add("Asphalt", (UMaterial *) RoadNode.Object);
  }
  if (RoadNodeAux.Succeeded()) {
    NameMaterialMap.Add("LaneMarkingYellow", (UMaterial *) RoadNodeAux.Object);
  }
  if (MarkingNode.Succeeded()) {
    NameMaterialMap.Add("LaneMarkingWhite", (UMaterial *) MarkingNode.Object);
  }
  if (TerrainNode.Succeeded()) {
    NameMaterialMap.Add("Grass", (UMaterial *) TerrainNode.Object);
  }
  if (ConcreteNode.Succeeded()) {
    NameMaterialMap.Add("Concrete", (UMaterial *) ConcreteNode.Object);
  }
}

const FString ACarlaEditorGameModeBase::ImportDirectory = "Import";
const FString ACarlaEditorGameModeBase::ExportDirectory = "Export";

void ACarlaEditorGameModeBase::BeginPlay() {
  Super::BeginPlay();

  DataExporter = NewObject<UDataExporter>();

  ChangeMenuWidget(StartingWidgetClass);

  // Load OpenDrive file
  ImportOpenDriveFile();

  // Load level
  LoadMeshes();
}

void ACarlaEditorGameModeBase::ChangeMenuWidget(TSubclassOf<UEditorGUI> NewWidgetClass) {
  if (EditorGUI != nullptr) {
    EditorGUI->RemoveFromViewport();
    EditorGUI = nullptr;
  }
  if (NewWidgetClass != nullptr) {
    EditorGUI = CreateWidget<UEditorGUI>(GetWorld(), NewWidgetClass);
    if (EditorGUI != nullptr) {
      EditorGUI->AddToViewport();
    }
  }
}

void ACarlaEditorGameModeBase::LoadMeshes() {
  TArray<FString> FileNames = GetAllFilesInDirectory(ImportDirectory, "", "fbx");
  UE_LOG(LogTemp, Warning, TEXT("Names length: %d\n"), FileNames.Num());
  for (auto &FileName : FileNames) {
    FReturnedData Data = ULoaderBPFunctionLibrary::LoadMesh(ImportDirectory + "/" + FileName,
        EPathType::Relative);
    FString Outcome = Data.bSuccess ? "True" : "False";
    if (Data.bSuccess) {
      UE_LOG(LogTemp, Log, TEXT("FBX file Loaded successfully\n"));
    } else {
      UE_LOG(LogTemp, Error, TEXT("Error occured loading the FBX file\n"));
    }

    for (auto &MeshInfo : Data.MeshInfoArray) {
      UE_LOG(LogTemp, Warning, TEXT("Mesh Name: %s\n"), *MeshInfo.Name);
      UE_LOG(LogTemp, Warning, TEXT("Mesh Material Name: %s\n"), *MeshInfo.MaterialName);
      for (const TPair<FString, UMaterial *> &pair : NameMaterialMap) {
        if (MeshInfo.MaterialName.Contains("LaneMarking", ESearchCase::IgnoreCase) &&
            !MeshInfo.MaterialName.Contains("Yellow", ESearchCase::IgnoreCase)) {
          MeshInfo.MaterialName = "LaneMarkingWhite";
        }
        if (MeshInfo.MaterialName.Contains(pair.Key, ESearchCase::IgnoreCase)) {
          UMaterial *Material = pair.Value;
          AProceduralActor *ProceduralActor = GetWorld()->SpawnActor<AProceduralActor>(
              AProceduralActor::StaticClass(),
              FVector::ZeroVector,
              FRotator::ZeroRotator);
          ProceduralActor->CreateMeshSection(MeshInfo, Material);
          break;
        }
      }
    }
  }
}

// Gets all the files in a directory
TArray<FString> ACarlaEditorGameModeBase::GetAllFilesInDirectory(
    const FString Directory,
    const FString OnlyFilesStartingWith,
    const FString OnlyFilesWithExtension) {
  FString DirectoryPath = FPaths::Combine(FPaths::ProjectDir(), Directory);
  // Get all files in directory
  TArray<FString> DirectoriesToSkip;
  IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
  FLocalTimestampDirectoryVisitor Visitor(PlatformFile, DirectoriesToSkip, DirectoriesToSkip, false);
  PlatformFile.IterateDirectory(*DirectoryPath, Visitor);
  TArray<FString> Files;

  for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt) {
    const FString FilePath = TimestampIt.Key();
    const FString FileName = FPaths::GetCleanFilename(FilePath);
    bool AddFile = true;

    // Check if filename starts with required characters
    if (!OnlyFilesStartingWith.IsEmpty()) {
      const FString left = FileName.Left(OnlyFilesStartingWith.Len());

      if (!(FileName.Left(OnlyFilesStartingWith.Len()).Equals(OnlyFilesStartingWith))) {
        AddFile = false;
      }
    }

    // Check if file extension is required characters
    if (!OnlyFilesWithExtension.IsEmpty()) {
      if (!(FPaths::GetExtension(FileName, false).Equals(OnlyFilesWithExtension, ESearchCase::IgnoreCase))) {
        AddFile = false;
      }
    }

    if (AddFile) {
      Files.Add(FileName);
    }
  }

  return Files;
}

void ACarlaEditorGameModeBase::ImportOpenDriveFile() {
  TArray<FString> FileNames = GetAllFilesInDirectory(ImportDirectory, "", "xodr");
  for (auto &FileName : FileNames) {
    FString FilePath = ImportDirectory + "/" + FileName;
    FilePath = FPaths::Combine(FPaths::ProjectDir(), FilePath);
    UE_LOG(LogTemp, Warning, TEXT("File Path: %s\n"), *FilePath);
    if (DataExporter != nullptr && !DataExporter->IsPendingKill()) {
      UE_LOG(LogTemp, Warning, TEXT("Here\n"));
      DataExporter->Load(FilePath);
    }
    break;
  }
}

void ACarlaEditorGameModeBase::ExportOpenDriveFile() {
  TArray<FString> FileNames = GetAllFilesInDirectory(ImportDirectory, "", "xodr");
  for (auto &FileName : FileNames) {
    FString FilePath = ExportDirectory + "/" + FileName;
    FilePath = FPaths::Combine(FPaths::ProjectDir(), FilePath);
    if (DataExporter != nullptr && !DataExporter->IsPendingKill()) {
      UE_LOG(LogTemp, Warning, TEXT("Here\n"));
      DataExporter->Save(FilePath);
      // Reload OpenDrive file
      ImportOpenDriveFile();
    }
    break;
  }
}
