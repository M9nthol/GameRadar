#include "SimpleGameEndWidget.h"
#include "Components/TextBlock.h"

USimpleGameEndWidget::USimpleGameEndWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void USimpleGameEndWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // Изначально скрываем виджет
    SetVisibility(ESlateVisibility::Hidden);
}

void USimpleGameEndWidget::ShowVictoryMessage()
{
    if (MessageText)
    {
        MessageText->SetText(FText::FromString(TEXT("Победа!")));
    }
    SetVisibility(ESlateVisibility::Visible);
}

void USimpleGameEndWidget::ShowDefeatMessage()
{
    if (MessageText)
    {
        MessageText->SetText(FText::FromString(TEXT("Поражение!")));
    }
    SetVisibility(ESlateVisibility::Visible);
}

void USimpleGameEndWidget::HideMessage()
{
    SetVisibility(ESlateVisibility::Hidden);
} 