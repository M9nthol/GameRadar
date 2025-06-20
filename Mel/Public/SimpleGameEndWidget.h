#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimpleGameEndWidget.generated.h"

UCLASS()
class MEL_API USimpleGameEndWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    USimpleGameEndWidget(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowVictoryMessage();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowDefeatMessage();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideMessage();

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MessageText;
}; 