// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialTextMetadata.h"

#include "Components/DynamicMeshComponent.h"

ATutorialSizeText::ATutorialSizeText() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.133f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ATutorialSizeText::BeginPlay() {
	Super::BeginPlay();

	InitializeMeshPool(8);
}

void ATutorialSizeText::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ATutorialSizeText::AssignText(AActor* InActor, const TSharedPtr<FTutorialTextMetadata>& InText) {
	if (!IsValid(InActor)) {
		UE_LOG(LogTemp, Error, TEXT("ATutorialSizeText::AssignText: InActor is not valid."));
		return;
	}

	InActor->OnDestroyed.AddDynamic(this, &ATutorialSizeText::OnActorDestroyed);

	UDynamicMeshComponent* Mesh = nullptr;
	if (UnassignedMeshes.Num() == 0) {
		Mesh = CreateMesh();
		UnassignedMeshes.Add(Mesh);
	}

	do {
		Mesh = UnassignedMeshes.Pop();
	} while (!IsValid(Mesh));

	RebuildMesh(Mesh, InText);

	AssignedMeshes.Add(Mesh);
	InText->SetMesh(Mesh);

	if (auto* Container = ActorToTextMap.Find(InActor)) {
		Container->Add(InText);
	} else {
		ActorToTextMap.Add(InActor, TArray{InText});
	}

	if (auto** RegisteredText = TextToMeshMap.Find(InText)) {
		UE_LOG(LogTemp, Error, TEXT("ATutorialSizeText::AssignText: Text is already registered."));
	} else {
		TextToMeshMap.Add(InText, Mesh);
	}

	const auto NewTransform = FTransform(InText->GetRotation(), InText->GetLocation(), FVector::OneVector);
	Mesh->SetWorldTransform(NewTransform);
	Mesh->SetVisibility(true);
	Mesh->SetHiddenInGame(false);
}

const FTransform EmptyTransform = FTransform(FRotator::ZeroRotator, FVector(UE_LARGE_WORLD_MAX, UE_LARGE_WORLD_MAX, UE_LARGE_WORLD_MAX),
                                             FVector::OneVector);

void ATutorialSizeText::ReleaseText(AActor* InActor) {
	if (!IsValid(InActor)) {
		UE_LOG(LogTemp, Error, TEXT("ATutorialSizeText::ReleaseText: InActor is not valid."));
		return;
	}

	if (auto* Container = ActorToTextMap.Find(InActor)) {
		for (auto It = Container->CreateIterator(); It; ++It) {
			if (auto Instance = *It; Instance.IsValid()) {
				if (auto** RegisteredInstance = TextToMeshMap.Find(Instance)) {
					(*RegisteredInstance)->SetWorldTransform(EmptyTransform);
					(*RegisteredInstance)->SetVisibility(false);
					(*RegisteredInstance)->SetHiddenInGame(true);
					UnassignedMeshes.Add(*RegisteredInstance);
					AssignedMeshes.Remove(*RegisteredInstance);
				} else {
					UE_LOG(LogTemp, Error, TEXT("ATutorialSizeText::ReleaseText: Text is not registered."));
				}

				Instance->SetMesh(nullptr);
				Instance->SetActor(nullptr);

				It.RemoveCurrent();
			}
		}
	}

	ActorToTextMap.Remove(InActor);
}

void ATutorialSizeText::OnActorDestroyed(AActor* InActor) {
	ReleaseText(InActor);
}

UDynamicMeshComponent* ATutorialSizeText::CreateMesh() {
	auto* Component = NewObject<UDynamicMeshComponent>(this);
	Component->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Component->PrimaryComponentTick.bCanEverTick = false;
	Component->SetMobility(EComponentMobility::Movable);
	Component->SetWorldTransform(EmptyTransform);
	Component->SetVisibility(false);
	Component->SetHiddenInGame(true);
	Component->SetCastShadow(false);
	Component->SetCanEverAffectNavigation(false);
	Component->SetGenerateOverlapEvents(false);
	Component->SetSimulatePhysics(false);
	Component->SetDeferredCollisionUpdatesEnabled(true);
	Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Component->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Component->SetCollisionResponseToAllChannels(ECR_Ignore);
	Component->SetMaterial(0, Material);
	Component->CreationMethod = EComponentCreationMethod::Instance;
	Component->RegisterComponent();
	Meshes.Add(Component);
	return Component;
}

void ATutorialSizeText::InitializeMeshPool(int32 InCount) {
	for (int32 i = 0; i < InCount; ++i) {
		auto* Mesh = CreateMesh();
		UnassignedMeshes.Add(Mesh);
	}
}

void ATutorialSizeText::RebuildMesh(UDynamicMeshComponent* InMesh, const TSharedPtr<FTutorialTextMetadata>& InText) {
	if (!IsValid(InMesh)) {
		UE_LOG(LogTemp, Error, TEXT("ATutorialSizeText::RebuildMesh: InMesh is not valid."));
		return;
	}

	if (!InText) {
		UE_LOG(LogTemp, Error, TEXT("ATutorialSizeText::RebuildMesh: InText is not valid."));
		return;
	}

	auto* Mesh = InMesh->GetMesh();
	if (!Mesh) {
		UE_LOG(LogTemp, Error, TEXT("ATutorialSizeText::RebuildMesh: Mesh is not valid."));
		return;
	}

	Mesh->Clear();

	TArray<FVector> VerticesWS;
	TArray<UE::Geometry::FIndex3i> Triangles;
	TArray<FVector2f> UVs;
	TArray<FVector3f> Normals;

	TriangulateMesh(InMesh, InText, VerticesWS, Triangles, UVs, Normals);
	Mesh->EnableTriangleGroups();
	Mesh->EnableAttributes();

	auto* UvOverlay = Mesh->Attributes()->PrimaryUV();
	auto* NormalOverlay = Mesh->Attributes()->PrimaryNormals();

	for (int i = 0; i < UVs.Num(); ++i) {
		UvOverlay->AppendElement(UVs[i]);
	}

	for (int i = 0; i < Normals.Num(); ++i) {
		NormalOverlay->AppendElement(Normals[i]);
	}

	const auto Transform = GetTransform();
	for (const auto& VertexWS : VerticesWS) {
		const auto Vertex = Transform.InverseTransformPosition(VertexWS);
		Mesh->AppendVertex(Vertex);
	}

	for (auto i = 0; i < Triangles.Num(); ++i) {
		const int TriangleId = Mesh->AppendTriangle(Triangles[i]);
		UvOverlay->SetTriangle(TriangleId, Triangles[i]);
		NormalOverlay->SetTriangle(TriangleId, Triangles[i]);
	}

	InMesh->NotifyMeshUpdated();
}

void ATutorialSizeText::TriangulateMesh(const UDynamicMeshComponent* InMesh, const TSharedPtr<FTutorialTextMetadata>& InText,
                                        TArray<FVector>& OutVerticesWS, TArray<UE::Geometry::FIndex3i>& OutTriangles, TArray<FVector2f>& OutUVs,
                                        TArray<FVector3f>& OutNormals) const {
	if (!IsValid(InMesh)) {
		UE_LOG(LogTemp, Error, TEXT("ATutorialSizeText::TriangulateMesh: InMesh is not valid."));
		return;
	}

	if (!InText) {
		UE_LOG(LogTemp, Error, TEXT("ATutorialSizeText::TriangulateMesh: InText is not valid."));
		return;
	}

	constexpr auto LetterWidth = 20.0;
	constexpr auto LetterHeight = 20.0;
	constexpr auto LetterHorizontalSpacing = 0.25;
	constexpr auto LetterVerticalOffset = 10.0;
	const int32 LetterCount = InText->GetText().Len();
	OutVerticesWS.Reset(LetterCount * 4);
	OutTriangles.Reset(LetterCount * 2);
	OutUVs.Reset(LetterCount * 4);
	OutNormals.Reset(LetterCount * 4);

	const auto Base = FVector{
		-LetterWidth * LetterCount * LetterHorizontalSpacing - LetterHorizontalSpacing, LetterHeight - LetterVerticalOffset, BaseHeight
	};

	const TArray Letters = {
		TEXT('0'), TEXT('1'), TEXT('2'), TEXT('3'),
		TEXT('4'), TEXT('5'), TEXT('6'), TEXT('7'),
		TEXT('8'), TEXT('9'), TEXT(','), TEXT('.'),
		TEXT('+'), TEXT('-'), TEXT('×'), TEXT('÷'),
	};

	for (auto i = 0; i < LetterCount; ++i) {
		const auto LetterCenter = -(LetterWidth - 1.0) * i / 2.0;
		const auto Letter = InText->GetText()[i];

		const auto X = Base.X + i * LetterWidth - i * LetterHorizontalSpacing + LetterCenter;
		const auto Y = Base.Y + LetterVerticalOffset;
		const auto Z = Base.Z;

		const auto V0 = FVector(X, Y, Z);
		const auto V1 = FVector(X, Y + LetterHeight, Z);
		const auto V2 = FVector(X + LetterWidth, Y, Z);
		const auto V3 = FVector(X + LetterWidth, Y + LetterHeight, Z);

		const auto VertexIndex = OutVerticesWS.Num();

		OutVerticesWS.Add(V0);
		OutVerticesWS.Add(V1);
		OutVerticesWS.Add(V2);
		OutVerticesWS.Add(V3);

		OutTriangles.Add(UE::Geometry::FIndex3i(VertexIndex, VertexIndex + 1, VertexIndex + 3));
		OutTriangles.Add(UE::Geometry::FIndex3i(VertexIndex, VertexIndex + 3, VertexIndex + 2));

		const auto LetterIndex = Letters.Find(Letter);
		if (LetterIndex == INDEX_NONE) {
			OutUVs.Add(FVector2f(0.0, 0.0));
			OutUVs.Add(FVector2f(0.0, 1.0));
			OutUVs.Add(FVector2f(1.0, 0.0));
			OutUVs.Add(FVector2f(1.0, 1.0));
			continue;
		}

		const auto U0 = FVector2f(LetterIndex % 4 * 0.25f, LetterIndex / 4 * 0.25f);
		const auto U1 = FVector2f(LetterIndex % 4 * 0.25f, (LetterIndex / 4 + 1) * 0.25f);
		const auto U2 = FVector2f((LetterIndex % 4 + 1) * 0.25f, LetterIndex / 4 * 0.25f);
		const auto U3 = FVector2f((LetterIndex % 4 + 1) * 0.25f, (LetterIndex / 4 + 1) * 0.25f);

		OutUVs.Add(U0);
		OutUVs.Add(U1);
		OutUVs.Add(U2);
		OutUVs.Add(U3);

		const auto N = FVector3f(0.0, 0.0, 1.0);
		OutNormals.Add(N);
		OutNormals.Add(N);
		OutNormals.Add(N);
		OutNormals.Add(N);
	}
}
