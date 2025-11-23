// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "A1HpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class A1_API UA1HpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UA1HpBarWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateHp(float NewCurrentHp, float NewMaxHp);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar;

	float CurrentHp;
	float MaxHp;
};
