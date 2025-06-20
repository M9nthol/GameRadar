#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MelGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    Playing,
    Victory,
    Defeat
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EGameState, NewState);

UCLASS()
class MEL_API AMelGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMelGameMode();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Game")
    void OnMissleDestroyed();

    UFUNCTION(BlueprintCallable, Category = "Game")
    void OnMissleReachedTarget();

    UFUNCTION(BlueprintCallable, Category = "Game")
    void CheckGameEnd();

    UFUNCTION(BlueprintCallable, Category = "Game")
    void EndGame(EGameState EndState);

    UPROPERTY(BlueprintAssignable, Category = "Game")
    FOnGameStateChanged OnGameStateChanged;

    UPROPERTY(BlueprintReadOnly, Category = "Game")
    EGameState CurrentGameState;

    UPROPERTY(BlueprintReadOnly, Category = "Game")
    int32 TotalMissles;

    UPROPERTY(BlueprintReadOnly, Category = "Game")
    int32 DestroyedMissles;

    UPROPERTY(BlueprintReadOnly, Category = "Game")
    bool bMissleReachedTarget;

private:
    void InitializeGame();
}; 