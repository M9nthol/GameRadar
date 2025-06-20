#include "AAActor.h"
#include "RadarActor.h"
#include "MissleActor.h"
#include "AAProjectileActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

AAAActor::AAAActor()
{
    PrimaryActorTick.bCanEverTick = true;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    RadarRef = nullptr;
    TimeSinceLastFire = 0.0f;
}

void AAAActor::BeginPlay()
{
    Super::BeginPlay();
    TimeSinceLastFire = 0.0f;
    
    // Автоматически найти радар, если не установлен
    if (!RadarRef)
    {
        TArray<AActor*> FoundRadars;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARadarActor::StaticClass(), FoundRadars);
        if (FoundRadars.Num() > 0)
        {
            RadarRef = Cast<ARadarActor>(FoundRadars[0]);
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("ПВО: Автоматически найден радар"));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ПВО: Радар не найден на уровне!"));
        }
    }
}

void AAAActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TimeSinceLastFire += DeltaTime;
    
    // Очищаем уничтоженные ракеты из списка
    CleanupDestroyedMissiles();
    
    TryFireAtMissile();
}

void AAAActor::SetRadar(ARadarActor* Radar)
{
    RadarRef = Radar;
}

AMissleActor* AAAActor::FindTargetMissile()
{
    if (!RadarRef) 
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("ПВО: Нет ссылки на радар!"));
        return nullptr;
    }
    
    // Получаем только ракеты, обнаруженные 3 раза
    TArray<AMissleActor*> ValidMissiles = RadarRef->GetMissilesDetectedThreeTimes();
    if (ValidMissiles.Num() == 0) 
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("ПВО: Нет ракет, обнаруженных 3 раза"));
        return nullptr;
    }
    
    // Фильтруем ракеты, по которым уже стреляли
    TArray<AMissleActor*> AvailableMissiles;
    for (AMissleActor* Missile : ValidMissiles)
    {
        if (Missile && Missile->IsValidLowLevel() && !FiredAtMissiles.Contains(Missile))
        {
            AvailableMissiles.Add(Missile);
        }
    }
    
    if (AvailableMissiles.Num() == 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("ПВО: Все обнаруженные ракеты уже обстреливаются"));
        return nullptr;
    }
    
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("ПВО: Найдено %d новых ракет для стрельбы"), AvailableMissiles.Num()));
    
    // Выбираем ближайшую ракету из доступных
    float MinDist = FLT_MAX;
    AMissleActor* Closest = nullptr;
    for (AMissleActor* Missile : AvailableMissiles)
    {
        if (Missile && Missile->IsValidLowLevel())
        {
            float Dist = FVector::Dist(Missile->GetActorLocation(), GetActorLocation());
            if (Dist < MinDist)
            {
                MinDist = Dist;
                Closest = Missile;
            }
        }
    }
    return Closest;
}

void AAAActor::TryFireAtMissile()
{
    if (TimeSinceLastFire < FireInterval) 
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("ПВО: Ожидание %.1f сек"), FireInterval - TimeSinceLastFire));
        return;
    }
    
    AMissleActor* TargetMissile = FindTargetMissile();
    if (!TargetMissile) 
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Orange, TEXT("ПВО: Нет цели для стрельбы"));
        return;
    }
    
    if (!ProjectileClass) 
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ПВО: Не указан класс снаряда!"));
        return;
    }

    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = (TargetMissile->GetActorLocation() - SpawnLocation).Rotation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    // Спавним снаряд
    AAAProjectileActor* Projectile = GetWorld()->SpawnActor<AAAProjectileActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
    if (Projectile)
    {
        Projectile->InitProjectile(TargetMissile, InitialForwardDistance, ProjectileSpeed);
        TimeSinceLastFire = 0.0f;
        
        // Добавляем ракету в список обстреливаемых
        FiredAtMissiles.Add(TargetMissile);
        
        // Воспроизводим звук выстрела
        PlayFireSound();
        
        // Отладочное сообщение
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("ПВО: Запуск снаряда по ракете! Всего обстреливается: %d"), FiredAtMissiles.Num()));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ПВО: Ошибка создания снаряда!"));
    }
}

void AAAActor::CleanupDestroyedMissiles()
{
    // Удаляем уничтоженные ракеты из списка
    for (int32 i = FiredAtMissiles.Num() - 1; i >= 0; i--)
    {
        if (!FiredAtMissiles[i] || !FiredAtMissiles[i]->IsValidLowLevel())
        {
            FiredAtMissiles.RemoveAt(i);
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("ПВО: Ракета уничтожена, удалена из списка"));
        }
    }
}

void AAAActor::PlayFireSound()
{
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }
} 