#include "MissleSpawner.h"
#include "MissleActor.h"
#include "MelGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AMissleSpawner::AMissleSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMissleSpawner::BeginPlay()
{
    Super::BeginPlay();

    for (int i = 0; i < MissleCount; ++i)
    {
        SpawnMissle();
    }
}

void AMissleSpawner::SpawnMissle()
{
    if (!MissleClass) return;

    FVector SpawnLocation = GetRandomEdgePosition(MapHalfSize);
    AMissleActor* Missle = GetWorld()->SpawnActor<AMissleActor>(MissleClass, SpawnLocation, FRotator::ZeroRotator);
    
    if (Missle)
    {
        // Подписываемся на события ракеты
        Missle->OnMissleDestroyed.AddDynamic(this, &AMissleSpawner::OnMissleDestroyed);
        Missle->OnMissleReachedTarget.AddDynamic(this, &AMissleSpawner::OnMissleReachedTarget);
    }
}

void AMissleSpawner::OnMissleDestroyed()
{
    // Уведомляем GameMode о уничтожении ракеты
    AMelGameMode* GameMode = Cast<AMelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->OnMissleDestroyed();
    }
}

void AMissleSpawner::OnMissleReachedTarget()
{
    // Уведомляем GameMode о достижении ракетой цели
    AMelGameMode* GameMode = Cast<AMelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->OnMissleReachedTarget();
    }
}

FVector AMissleSpawner::GetRandomEdgePosition(float Distance)
{
    int32 Edge = FMath::RandRange(0, 3);
    float X = 0.f, Y = 0.f;
    switch (Edge)
    {
    case 0: X = Distance; Y = FMath::RandRange(-Distance, Distance); break;
    case 1: X = -Distance; Y = FMath::RandRange(-Distance, Distance); break;
    case 2: Y = Distance; X = FMath::RandRange(-Distance, Distance); break;
    case 3: Y = -Distance; X = FMath::RandRange(-Distance, Distance); break;
    }

    return FVector(X, Y, 100.f);
}

