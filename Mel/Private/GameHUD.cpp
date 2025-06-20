#include "GameHUD.h"
#include "SimpleGameEndWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"

AGameHUD::AGameHUD()
{
}

void AGameHUD::BeginPlay()
{
    Super::BeginPlay();

    // Создаем виджет конца игры
    if (GameEndWidgetClass)
    {
        GameEndWidget = CreateWidget<USimpleGameEndWidget>(GetWorld(), GameEndWidgetClass);
        if (GameEndWidget)
        {
            GameEndWidget->AddToViewport();
        }
    }
}

void AGameHUD::ShowGameEndMessage(bool bIsVictory)
{
    if (GameEndWidget)
    {
        if (bIsVictory)
        {
            GameEndWidget->ShowVictoryMessage();
        }
        else
        {
            GameEndWidget->ShowDefeatMessage();
        }
    }
}

void AGameHUD::HideGameEndMessage()
{
    if (GameEndWidget)
    {
        GameEndWidget->HideMessage();
    }
} 