// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/A1HpBarWidget.h"
#include "Components/ProgressBar.h"

UA1HpBarWidget::UA1HpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UA1HpBarWidget::UpdateHp(float NewCurrentHp, float NewMaxHp)
{
	CurrentHp = NewCurrentHp;
	MaxHp = NewMaxHp;

	if (HpBar)
	{
		HpBar->SetPercent(CurrentHp/MaxHp);
	}
}
