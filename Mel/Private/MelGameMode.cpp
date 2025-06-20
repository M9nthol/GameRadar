#include "MelGameMode.h"
#include "MissleSpawner.h"
#include "MissleActor.h"
#include "GameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AMelGameMode::AMelGameMode()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentGameState = EGameState::Playing;
    TotalMissles = 0;
    DestroyedMissles = 0;
    bMissleReachedTarget = false;
    
    // Устанавливаем HUD класс
    HUDClass = AGameHUD::StaticClass();
}

void AMelGameMode::BeginPlay()
{
    Super::BeginPlay();
    InitializeGame();
}

void AMelGameMode::InitializeGame()
{
    // Находим MissleSpawner и получаем количество ракет
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMissleSpawner::StaticClass(), FoundActors);
    
    if (FoundActors.Num() > 0)
    {
        AMissleSpawner* Spawner = Cast<AMissleSpawner>(FoundActors[0]);
        if (Spawner)
        {
            TotalMissles = Spawner->MissleCount;
        }
    }
}

void AMelGameMode::OnMissleDestroyed()
{
    DestroyedMissles++;
    CheckGameEnd();
}

void AMelGameMode::OnMissleReachedTarget()
{
    bMissleReachedTarget = true;
    EndGame(EGameState::Defeat);
}

void AMelGameMode::CheckGameEnd()
{
    if (DestroyedMissles >= TotalMissles && !bMissleReachedTarget)
    {
        EndGame(EGameState::Victory);
    }
}

void AMelGameMode::EndGame(EGameState EndState)
{
    if (CurrentGameState != EGameState::Playing)
        return;

    CurrentGameState = EndState;
    OnGameStateChanged.Broadcast(EndState);

    // Показываем сообщение через HUD
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        AGameHUD* HUD = Cast<AGameHUD>(PC->GetHUD());
        if (HUD)
        {
            bool bIsVictory = (EndState == EGameState::Victory);
            HUD->ShowGameEndMessage(bIsVictory);
        }
    }
} 