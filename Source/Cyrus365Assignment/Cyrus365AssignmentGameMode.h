// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/IHttpRequest.h"
#include "Cyrus365AssignmentGameMode.generated.h"

UCLASS(minimalapi)
class ACyrus365AssignmentGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACyrus365AssignmentGameMode();
	virtual void BeginPlay() override;
	void AddScore(int32 Points);
	int32 Score;

protected:
	void FetchJsonData();
	void OnJsonDownloaded(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void ParseJson(const FString& JsonString);
	void SpawnBoxes(const TArray<TSharedPtr<FJsonValue>>& ObjectsArray, TSharedPtr<FJsonObject> JsonObject);
	void SpawnBox(const FString& Type, const FVector& Location, const FRotator& Rotation, const FVector& Scale, TSharedPtr<FJsonObject> JsonObject);

	
	UPROPERTY(EditAnywhere, Category = "Box")
	TSubclassOf<class ABoxActor> BoxClass;

	UPROPERTY()
	class UUserWidget* ScoreHUD;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<class UUserWidget> ScoreHUDClass;

	void UpdateHUD();
	
};



