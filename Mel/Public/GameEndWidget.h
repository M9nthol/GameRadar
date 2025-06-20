#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndWidget.generated.h"

UCLASS()
class MEL_API UGameEndWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UGameEndWidget(const FObjectInitializer& ObjectInitializer);

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

    UPROPERTY(meta = (BindWidget))
    class UButton* RestartButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;

    UFUNCTION()
    void OnRestartClicked();

    UFUNCTION()
    void OnQuitClicked();
}; 