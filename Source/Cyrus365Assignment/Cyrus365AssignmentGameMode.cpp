#include "Cyrus365AssignmentGameMode.h"
#include "BoxActor.h"
#include "HttpModule.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "UObject/ConstructorHelpers.h"

ACyrus365AssignmentGameMode::ACyrus365AssignmentGameMode()
	: Super()
{

    // Initialize Score to 0
    Score = 0;
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void ACyrus365AssignmentGameMode::BeginPlay()
{
    Super::BeginPlay();
    // Create and display the HUD
    if (ScoreHUDClass)
    {
        ScoreHUD = CreateWidget<UUserWidget>(GetWorld(), ScoreHUDClass);
        if (ScoreHUD)
        {
            ScoreHUD->AddToViewport();
            UpdateHUD();
        }
    }
    FetchJsonData();
}

void ACyrus365AssignmentGameMode::FetchJsonData()
{
    UE_LOG(LogTemp, Log, TEXT("Fetching JSON data..."));
    FString URL = "https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json";
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ACyrus365AssignmentGameMode::OnJsonDownloaded);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb("GET");
    HttpRequest->ProcessRequest();
}

void ACyrus365AssignmentGameMode::OnJsonDownloaded(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        FString JsonString = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("JSON Downloaded: %s"), *JsonString);
        ParseJson(JsonString);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to download JSON file."));
    }
}

void ACyrus365AssignmentGameMode::ParseJson(const FString& JsonString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>>* ObjectsArray;
        if (JsonObject->TryGetArrayField(TEXT("objects"), ObjectsArray))
        {
            // Pass JsonObject to SpawnBoxes
            SpawnBoxes(*ObjectsArray, JsonObject);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON."));
    }
}

void ACyrus365AssignmentGameMode::SpawnBoxes(const TArray<TSharedPtr<FJsonValue>>& ObjectsArray, TSharedPtr<FJsonObject> JsonObject)
{
    for (const TSharedPtr<FJsonValue>& ObjectValue : ObjectsArray)
    {
        const TSharedPtr<FJsonObject>& ObjectData = ObjectValue->AsObject();
        if (ObjectData.IsValid())
        {
            FString Type;
            if (ObjectData->TryGetStringField(TEXT("type"), Type))
            {
                const TSharedPtr<FJsonObject>* TransformObject;
                if (ObjectData->TryGetObjectField(TEXT("transform"), TransformObject))
                {
                    FVector Location;
                    FRotator Rotation;
                    FVector Scale;

                    const TArray<TSharedPtr<FJsonValue>>* LocationArray;
                    const TArray<TSharedPtr<FJsonValue>>* RotationArray;
                    const TArray<TSharedPtr<FJsonValue>>* ScaleArray;

                    if (TransformObject->Get()->TryGetArrayField(TEXT("location"), LocationArray) &&
                        TransformObject->Get()->TryGetArrayField(TEXT("rotation"), RotationArray) &&
                        TransformObject->Get()->TryGetArrayField(TEXT("scale"), ScaleArray))
                    {
                        Location = FVector(
                            static_cast<float>((*LocationArray)[0]->AsNumber()),
                            static_cast<float>((*LocationArray)[1]->AsNumber()),
                            static_cast<float>((*LocationArray)[2]->AsNumber())
                        );

                        Rotation = FRotator(
                            static_cast<float>((*RotationArray)[0]->AsNumber()),
                            static_cast<float>((*RotationArray)[1]->AsNumber()),
                            static_cast<float>((*RotationArray)[2]->AsNumber())
                        );

                        Scale = FVector(
                            static_cast<float>((*ScaleArray)[0]->AsNumber()),
                            static_cast<float>((*ScaleArray)[1]->AsNumber()),
                            static_cast<float>((*ScaleArray)[2]->AsNumber())
                        );
                        // Pass JsonObject to SpawnBox
                        SpawnBox(Type, Location, Rotation, Scale, JsonObject);
                    }
                }
            }
        }
    }
}

void ACyrus365AssignmentGameMode::SpawnBox(const FString& Type, const FVector& Location, const FRotator& Rotation, const FVector& Scale, TSharedPtr<FJsonObject> JsonObject)
{
    if (BoxClass)
    {
        UE_LOG(LogTemp, Log, TEXT("Spawning box of type: %s at location: %s"), *Type, *Location.ToString());
        FActorSpawnParameters SpawnParams;
        ABoxActor* NewBox = GetWorld()->SpawnActor<ABoxActor>(BoxClass, Location, Rotation, SpawnParams);
        if (NewBox)
        {
            UE_LOG(LogTemp, Log, TEXT("Box spawned successfully."));
            NewBox->SetActorScale3D(Scale);
            
            // Find the matching "type" in the JSON "types" array
            const TArray<TSharedPtr<FJsonValue>>* TypesArray;
            if (JsonObject->TryGetArrayField("types", TypesArray))
            {
                for (const TSharedPtr<FJsonValue>& TypeData : *TypesArray)
                {
                    TSharedPtr<FJsonObject> TypeObj = TypeData->AsObject();
                    FString TypeName;
                    if (TypeObj->TryGetStringField("name", TypeName) && TypeName == Type)
                    {
                        // Extract color, health, and score
                        const TArray<TSharedPtr<FJsonValue>>* ColorArray;
                        if (TypeObj->TryGetArrayField("color", ColorArray) && ColorArray->Num() == 3)
                        {
                            FLinearColor Color(
                                (*ColorArray)[0]->AsNumber() / 255.0f, // R
                                (*ColorArray)[1]->AsNumber() / 255.0f, // G
                                (*ColorArray)[2]->AsNumber() / 255.0f  // B
                            );

                            int32 Health = TypeObj->GetIntegerField("health");
                            int32 BoxScore = TypeObj->GetIntegerField("score"); // Renamed to BoxScore

                            // Initialize the box with JSON data
                            NewBox->InitializeBox(Type, Color, Health, BoxScore);
                            UE_LOG(LogTemp, Log, TEXT("Box initialized with Type: %s, Color: %s, Health: %d, Score: %d"), *Type, *Color.ToString(), Health, BoxScore);
                        }
                        else
                        {
                            UE_LOG(LogTemp, Error, TEXT("Invalid color data for type: %s"), *Type);
                        }
                        break;
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("No 'types' array found in JSON."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn box."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BoxClass is not assigned."));
    }
}

void ACyrus365AssignmentGameMode::AddScore(int32 Points)
{
    Score += Points;
    UpdateHUD();
}

void ACyrus365AssignmentGameMode::UpdateHUD()
{
    if (ScoreHUD)
    {
        // Update the HUD with the current score
        UTextBlock* ScoreText = Cast<UTextBlock>(ScoreHUD->GetWidgetFromName(TEXT("ScoreText")));
        if (ScoreText)
        {
            ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Score)));
        }
    }
}