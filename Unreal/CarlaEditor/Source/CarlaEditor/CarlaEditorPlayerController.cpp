// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "CarlaEditorPlayerController.h"
#include "CarlaEditorGameModeBase.h"
#include "ActorControl.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerInput.h"

ACarlaEditorPlayerController::ACarlaEditorPlayerController(const FObjectInitializer &ObjectInitializer)
  : Super(ObjectInitializer) {
  PrimaryActorTick.bCanEverTick = true;
  ControlledActor = nullptr;
}

void ACarlaEditorPlayerController::BeginPlay() {
  Super::BeginPlay();
  SetInputMode(FInputModeGameAndUI());
  GameMode = static_cast<ACarlaEditorGameModeBase *>(GetWorld()->GetAuthGameMode());
  EditorGUI = GameMode->GetEditorGUI();
  bShowMouseCursor = true;
}

void ACarlaEditorPlayerController::Tick(float DeltaSeconds) {
  Super::Tick(DeltaSeconds);
}

void ACarlaEditorPlayerController::SetupInputComponent() {
  Super::SetupInputComponent();

  UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("LeftMouseClick",
      EKeys::LeftMouseButton));
  UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("RightMouseClick",
      EKeys::RightMouseButton));
  UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("DeleteClick", EKeys::Delete));
  UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("DisplayGUI", EKeys::M));
  UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("DisplayActorTable", EKeys::T));
  UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ExitEditor", EKeys::P));
  UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MouseXMovement", EKeys::MouseX, 1.f));
  UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MouseYMovement", EKeys::MouseY, -1.f));

  InputComponent->BindAction("LeftMouseClick",
      IE_Pressed,
      this,
      &ACarlaEditorPlayerController::ConsumeLeftMouseButtonPressed);
  InputComponent->BindAction("LeftMouseClick",
      IE_Released,
      this,
      &ACarlaEditorPlayerController::ConsumeLeftMouseButtonReleased);
  InputComponent->BindAction("RightMouseClick",
      IE_DoubleClick,
      this,
      &ACarlaEditorPlayerController::ConsumeRightMouseButtonDoubleClick);
  InputComponent->BindAction("DeleteClick",
      IE_Pressed,
      this,
      &ACarlaEditorPlayerController::ConsumeDeleteClick);
  InputComponent->BindAction("DisplayGUI", IE_Pressed, this, &ACarlaEditorPlayerController::DisplayGUI);
  InputComponent->BindAction("DisplayActorTable",
      IE_Pressed,
      this,
      &ACarlaEditorPlayerController::DisplayActorTable);
  InputComponent->BindAction("ExitEditor", IE_Pressed, this, &ACarlaEditorPlayerController::ConsumeEscClick);
  InputComponent->BindAxis("MouseXMovement", this, &ACarlaEditorPlayerController::ConsumeMouseXMovement);
  InputComponent->BindAxis("MouseYMovement", this, &ACarlaEditorPlayerController::ConsumeMouseYMovement);
}

void ACarlaEditorPlayerController::SetControlledActor(AActor *InControlledActor) {
  if (ControlledActor != nullptr && !ControlledActor->IsPendingKill()) {
    OutlineControlledActor(false);
  }
  ControlledActor = InControlledActor;
  if (InControlledActor == nullptr) {
    EditorGUI->DisplayActorDetails(false);
    ActorControl->SetActorHiddenInGame(true);
  } else {
    EditorGUI->DisplayActorDetails(true);
    EditorGUI->UpdateActorDetails();
    OutlineControlledActor(true);
    PlaceActorControl();
    ActorControl->SetActorHiddenInGame(false);
  }
}

void ACarlaEditorPlayerController::OutlineControlledActor(bool Outline) {
  TArray<USceneComponent *> Components;
  ControlledActor->GetRootComponent()->GetChildrenComponents(true, Components);
  for (auto Component : Components) {
    // Custom depth working only for static mesh components.
    // Therefore we need to change the color of shape components instead.
    if (Component->GetClass()->IsChildOf(UShapeComponent::StaticClass())) {
      auto Shape = static_cast<UShapeComponent *>(Component);
      if (Outline) {
        Shape->ShapeColor = FColor(255, 255, 0, 255);
      } else {
        Shape->ShapeColor = FColor(223, 149, 157, 255);
      }
      // Inform the engine to update the rendering info
      Shape->MarkRenderStateDirty();
    } else {
      if (Component->GetClass()->IsChildOf(UPrimitiveComponent::StaticClass())) {
        auto Primitive = static_cast<UPrimitiveComponent *>(Component);
        Primitive->SetCustomDepthStencilValue(253);
        Primitive->SetRenderCustomDepth(Outline);
      }
    }
  }
}

UBoxComponent *ACarlaEditorPlayerController::GetControlledActorBoxComponent() {
  UBoxComponent *Output = nullptr;
  TArray<USceneComponent *> Components;
  ControlledActor->GetRootComponent()->GetChildrenComponents(true, Components);
  for (auto Component : Components) {
    if (Component->IsA(UBoxComponent::StaticClass())) {
      Output = static_cast<UBoxComponent *>(Component);
      break;
    }
  }
  return Output;
}

AActor *ACarlaEditorPlayerController::SpawnActor(UClass *ActorClass) {
  FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
  FVector ActorLocation = CameraLocation + PlayerCameraManager->GetCameraRotation().Vector() * 800.0f;
  AActor *SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, ActorLocation, FRotator::ZeroRotator);
  SetControlledActor(SpawnedActor);
  EditorGUI->SelectActor(ControlledActor);
  return SpawnedActor;
}

void ACarlaEditorPlayerController::ConsumeLeftMouseButtonPressed() {
  bLeftMouseDown = true;
  FHitResult Result;
  GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, Result);

  if (Result.Actor.IsValid()) {
    if (Result.Actor->IsA(AActorControl::StaticClass())) {
      ActorControl->SetIsHeld(true);
    } else {
      if (ControlledActor == nullptr || ControlledActor != Result.Actor.Get()) {
        SetControlledActor(Result.Actor.Get());
        EditorGUI->SelectActor(ControlledActor);
      }
    }
  }
}

void ACarlaEditorPlayerController::ConsumeLeftMouseButtonReleased() {
  bLeftMouseDown = false;
  ActorControl->Deselect();
  ActorControl->SetIsHeld(false);
}

void ACarlaEditorPlayerController::ConsumeRightMouseButtonDoubleClick() {
  if (!bLeftMouseDown) {
    EditorGUI->DeselectActor(ControlledActor);
    SetControlledActor(nullptr);
  }
}

void ACarlaEditorPlayerController::ConsumeDeleteClick() {
  if (ControlledActor != nullptr && !ControlledActor->IsPendingKill()) {
    EditorGUI->DeleteActor(ControlledActor);
  }
}

void ACarlaEditorPlayerController::DeleteControlledActor() {
  ControlledActor->Destroy();
  EditorGUI->DisplayActorDetails(false);
  ActorControl->Deselect();
  ActorControl->SetIsHeld(false);
  ActorControl->SetActorHiddenInGame(true);
}

void ACarlaEditorPlayerController::DisplayGUI() {
  EditorGUI->DisplayGUI();
}

void ACarlaEditorPlayerController::DisplayActorTable() {
  EditorGUI->DisplayActorTable();
}

void ACarlaEditorPlayerController::ConsumeEscClick() {
  FGenericPlatformMisc::RequestExit(false);
}

void ACarlaEditorPlayerController::ConsumeMouseXMovement(float Value) {
  if (bLeftMouseDown && ControlledActor != nullptr && Value != 0.0f) {
    FVector Direction = ActorControl->GetDirection();
    if (Direction.IsZero()) {
      GetPawn()->AddControllerYawInput(Value);
    } else {
      if (Direction.X == 1.0f || Direction.Y == 1.0f) {
        // The sign of the multiplier will be determined using the outer product
        // The Z access can be ignored to make it a 2D problem
        FVector P1 = ActorControl->GetActorLocation();
        FVector P2 = P1 + Direction * 2.0f;
        FVector P3 = PlayerCameraManager->GetCameraLocation();
        float OP = (P3.X - P1.X) * (P2.Y - P1.Y) - (P3.Y - P1.Y) * (P2.X - P1.X);

        float multiplier = OP <= 0 ? 1.0f : -1.0f;

        FVector ActorLocation = ControlledActor->GetActorLocation() + Direction * Value * 45.0f * multiplier;
        ControlledActor->SetActorLocation(ActorLocation);
        PlaceActorControl();
        // Update the details of the selected actor in the GUI.
        EditorGUI->UpdateActorDetails();
      }
    }
  } else {
    GetPawn()->AddControllerYawInput(Value);
  }
}

void ACarlaEditorPlayerController::ConsumeMouseYMovement(float Value) {
  if (bLeftMouseDown && ControlledActor != nullptr && Value != 0.0f) {
    FVector Direction = ActorControl->GetDirection();
    if (Direction.IsZero()) {
      GetPawn()->AddControllerPitchInput(Value);
    } else {
      if (Direction.Z == 1.0f) {
        FVector ActorLocation = ControlledActor->GetActorLocation() + Direction * Value * -45.0f;
        ControlledActor->SetActorLocation(ActorLocation);
        PlaceActorControl();
        // Update the details of the selected actor in the GUI.
        EditorGUI->UpdateActorDetails();
      }
    }
  } else {
    GetPawn()->AddControllerPitchInput(Value);
  }
}

void ACarlaEditorPlayerController::PlaceActorControl() {
  ActorControl->SetActorLocation(ControlledActor->GetActorLocation() + FVector(50, 0, 0));
}
