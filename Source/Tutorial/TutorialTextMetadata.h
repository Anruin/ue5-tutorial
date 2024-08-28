// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IndexTypes.h"
#include "GameFramework/Actor.h"

#include "TutorialTextMetadata.generated.h"

class UDynamicMeshComponent;

class FTutorialTextMetadata {
	FVector Location;
	FRotator Rotation;
	FString Text;
	AActor* Owner;
	UDynamicMeshComponent* Mesh;

public:
	FTutorialTextMetadata() = default;

	void SetText(const FString& InText) {
		Text = InText;
	}

	void SetMesh(UDynamicMeshComponent* InMesh) {
		Mesh = InMesh;
	}

	void SetLocation(const FVector& InLocation) {
		Location = InLocation;
	}

	void SetRotation(const FRotator& InRotation) {
		Rotation = InRotation;
	}

	void SetActor(AActor* InActor) {
		Owner = InActor;
	}

	FVector GetLocation() const {
		return Location;
	}

	FRotator GetRotation() const {
		return Rotation;
	}

	FString GetText() const {
		return Text;
	}
};

UCLASS()
class ATutorialSizeText : public AActor {
	GENERATED_BODY()

public:
	ATutorialSizeText();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void AssignText(AActor* InActor, const TSharedPtr<FTutorialTextMetadata>& InText);
	void ReleaseText(AActor* InActor);

private:
	UFUNCTION()
	void OnActorDestroyed(AActor* InActor);

	UDynamicMeshComponent* CreateMesh();

	void InitializeMeshPool(int32 InCount);

	void RebuildMesh(UDynamicMeshComponent* InMesh, const TSharedPtr<FTutorialTextMetadata>& InText);

	void TriangulateMesh(const UDynamicMeshComponent* InMesh, const TSharedPtr<FTutorialTextMetadata>& InText,
	                     TArray<FVector>& OutVerticesWS, TArray<UE::Geometry::FIndex3i>& OutTriangles, TArray<FVector2f>& OutUVs,
	                     TArray<FVector3f>& OutNormals) const;

	UPROPERTY(EditDefaultsOnly, Category="Tutorial")
	double BaseHeight = 0.1;

	UPROPERTY(EditDefaultsOnly, Category="Tutorial")
	UMaterialInterface* Material = nullptr;

	UPROPERTY(VisibleInstanceOnly, Category="Tutorial")
	TArray<UDynamicMeshComponent*> Meshes;

	UPROPERTY(VisibleInstanceOnly, Category="Tutorial")
	TArray<UDynamicMeshComponent*> UnassignedMeshes;

	UPROPERTY(VisibleInstanceOnly, Category="Tutorial")
	TArray<UDynamicMeshComponent*> AssignedMeshes;

	TMap<TWeakObjectPtr<AActor>, TArray<TSharedPtr<FTutorialTextMetadata>>> ActorToTextMap;
	TMap<TSharedPtr<FTutorialTextMetadata>, UDynamicMeshComponent*> TextToMeshMap;
};
