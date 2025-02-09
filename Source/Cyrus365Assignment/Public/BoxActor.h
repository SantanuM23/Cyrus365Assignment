#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxActor.generated.h"

UCLASS()
class CYRUS365ASSIGNMENT_API ABoxActor : public AActor
{
	GENERATED_BODY()

public:
	ABoxActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void TakeDamage(int32 DamageAmount);
	void SetBoxColor(FLinearColor NewColor);
	void InitializeBox(FString BoxType, FLinearColor BoxColor, int32 BoxHealth, int32 BoxScore);
	void DestroyBox();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* BoxMesh;

	UPROPERTY(EditAnywhere, Category = "Box Properties")
	FString Type;

	UPROPERTY(EditAnywhere, Category = "Box Properties")
	FLinearColor Color;

	UPROPERTY(EditAnywhere, Category = "Box Properties")
	int32 Health;

	UPROPERTY(EditAnywhere, Category = "Box Properties")
	int32 Score;

private:
	UMaterialInstanceDynamic* DynamicMaterial;

};