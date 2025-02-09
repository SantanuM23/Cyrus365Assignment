#include "BoxActor.h"
#include "Components/StaticMeshComponent.h"
#include "Cyrus365Assignment/Cyrus365AssignmentGameMode.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

ABoxActor::ABoxActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	RootComponent = BoxMesh;


	// Load a default static mesh (e.g., a cube)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMesh.Succeeded())
	{
		BoxMesh->SetStaticMesh(CubeMesh.Object);
	}

	// Load a default material
	static ConstructorHelpers::FObjectFinder<UMaterial> BaseMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	if (BaseMaterial.Succeeded())
	{
		BoxMesh->SetMaterial(0, BaseMaterial.Object);
	}
	
	Health = 1;
}

void ABoxActor::BeginPlay()
{
	Super::BeginPlay();

	DynamicMaterial = BoxMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", Color);
	}
}

void ABoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoxActor::TakeDamage(int32 DamageAmount)
{
	Health -= DamageAmount;
	if (Health <= 0)
	{
		DestroyBox();
	}
}

void ABoxActor::SetBoxColor(FLinearColor NewColor)
{
	Color = NewColor;
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", Color);
	}
}

void ABoxActor::InitializeBox(FString BoxType, FLinearColor BoxColor, int32 BoxHealth, int32 BoxScore)
{
	Type = BoxType;
	Color = BoxColor;
	Health = BoxHealth;
	Score = BoxScore;
	
	// Create a dynamic material instance
	DynamicMaterial = BoxMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicMaterial)
	{
		// Set the color parameter
		DynamicMaterial->SetVectorParameterValue("BaseColor", BoxColor);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create dynamic material instance."));
	}
}

void ABoxActor::DestroyBox()
{
	// Notify the GameMode to update the score
	if (GetWorld())
	{
		AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
		if (GameMode)
		{
			// Cast to your custom GameMode and call the AddScore function
			ACyrus365AssignmentGameMode* CustomGameMode = Cast<ACyrus365AssignmentGameMode>(GameMode);
			if (CustomGameMode)
			{
				CustomGameMode->AddScore(Score);
			}
		}
	}

	// Destroy the box
	Destroy();
}