#include "GameEndWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UGameEndWidget::UGameEndWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UGameEndWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UGameEndWidget::OnRestartClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UGameEndWidget::OnQuitClicked);
    }

    // Изначально скрываем виджет
    SetVisibility(ESlateVisibility::Hidden);
}

void UGameEndWidget::ShowVictoryMessage()
{
    if (MessageText)
    {
        MessageText->SetText(FText::FromString(TEXT("Победа!")));
    }
    SetVisibility(ESlateVisibility::Visible);
}

void UGameEndWidget::ShowDefeatMessage()
{
    if (MessageText)
    {
        MessageText->SetText(FText::FromString(TEXT("Поражение!")));
    }
    SetVisibility(ESlateVisibility::Visible);
}

void UGameEndWidget::HideMessage()
{
    SetVisibility(ESlateVisibility::Hidden);
}

void UGameEndWidget::OnRestartClicked()
{
    // Перезапускаем игру
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetMapName()));
}

void UGameEndWidget::OnQuitClicked()
{
    // Выходим из игры
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->ConsoleCommand("quit");
    }
} 