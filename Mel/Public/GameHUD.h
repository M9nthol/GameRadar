#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

UCLASS()
class MEL_API AGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    AGameHUD();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class USimpleGameEndWidget> GameEndWidgetClass;

    UPROPERTY()
    class USimpleGameEndWidget* GameEndWidget;

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowGameEndMessage(bool bIsVictory);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideGameEndMessage();
}; 