// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CarlaEditorPlayerController.generated.h"

class ACarlaEditorGameModeBase;
class UEditorGUI;
class AActorControl;
class UBoxComponent;

/**
 *
 */
UCLASS()
class CARLAEDITOR_API ACarlaEditorPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:
		ACarlaEditorPlayerController(const FObjectInitializer &ObjectInitializer);

		virtual void BeginPlay() override;

		virtual void Tick(float DeltaSeconds) override;

		UFUNCTION(Category = "Editor Controller", BlueprintCallable)
  	AActor* GetControlledActor() const
		{
			return ControlledActor;
		}

		UFUNCTION(Category = "Editor Controller", BlueprintCallable)
  	void SetControlledActor(AActor* InControlledActor);

		UFUNCTION(Category = "Editor Controller", BlueprintCallable)
  	AActorControl* GetActorControl() const
		{
			return ActorControl;
		}

		UFUNCTION(BlueprintCallable, Category = "Editor Controller")
		UEditorGUI* GetEditorGUI() const
		{
			return EditorGUI;
		}

		UFUNCTION(Category = "Editor Controller", BlueprintCallable)
  	UBoxComponent* GetControlledActorBoxComponent();

		UFUNCTION(Category = "Editor Controller", BlueprintCallable)
		AActor* SpawnActor(UClass* ActorClass);

		UFUNCTION(Category = "Editor Controller", BlueprintCallable)
		void DeleteControlledActor();

	protected:
		virtual void SetupInputComponent() override;

		UFUNCTION(Category = "Editor Controller", BlueprintCallable)
		virtual void PlaceActorControl();

		UPROPERTY(Category = "Editor Controller", BlueprintReadWrite, VisibleAnywhere)
		AActorControl* ActorControl;

	private:
		void ConsumeLeftMouseButtonPressed();

		void ConsumeLeftMouseButtonReleased();

		void ConsumeRightMouseButtonDoubleClick();

		void ConsumeDeleteClick();

		void ConsumeMouseXMovement(float Value);

		void ConsumeMouseYMovement(float Value);

		void DisplayGUI();

		void DisplayActorTable();

		UPROPERTY(Category = "Editor Controller", VisibleAnywhere)
		UEditorGUI* EditorGUI;

		UPROPERTY(Category = "Editor Controller", VisibleAnywhere)
		AActor* ControlledActor;

		UPROPERTY(Category = "Editor Controller", VisibleAnywhere)
		bool bLeftMouseDown = false;

		ACarlaEditorGameModeBase* GameMode;
};
