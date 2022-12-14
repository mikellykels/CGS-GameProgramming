// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UObjectiveWorldSubsystem::CreateObjectiveWidget(TSubclassOf<UUserWidget> ObjectiveWidgetClass) 
{
	if (ObjectiveWidget == nullptr)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		ObjectiveWidget = CreateWidget<UUserWidget>(PlayerController, ObjectiveWidgetClass);
	}
}

void UObjectiveWorldSubsystem::DisplayObjectiveWidget()
{
	ensureMsgf(ObjectiveWidget, TEXT("UObjectiveWorldSubsystem::DisplayObjectiveWidget ObjectiveWidget is nullptr"));
	ObjectiveWidget->AddToViewport();
}

FString UObjectiveWorldSubsystem::GetCurrentObjectiveDescription()
{
	if (!Objectives.IsValidIndex(0) || Objectives[0]->GetState() == EObjectiveState::OS_Inactive)
	{
		return TEXT("N/A");
	}

	FString RetObjective = Objectives[0]->GetDescription();
	if (Objectives[0]->GetState() == EObjectiveState::OS_Completed)
	{
		RetObjective += TEXT("completed!");
	}
	return RetObjective;
}

float UObjectiveWorldSubsystem::GetScore()
{
	if (!Objectives.IsValidIndex(0) || Objectives[0]->GetState() == EObjectiveState::OS_Inactive)
	{
		return 0.0f;
	}
	if (Objectives[0]->GetState() == EObjectiveState::OS_Completed)
	{
		Score = Score + 1;
	}
	return Score;
}

void UObjectiveWorldSubsystem::AddObjective(UObjectiveComponent* ObjectiveComponent)
{
	check(ObjectiveComponent);

	size_t PrevSize = Objectives.Num();
	Objectives.AddUnique(ObjectiveComponent);
	if (Objectives.Num() > PrevSize)
	{
		ObjectiveComponent->OnStateChanged().AddUObject(this, &UObjectiveWorldSubsystem::OnObjectiveStateChanged);
	}
}

void UObjectiveWorldSubsystem::RemoveObjective(UObjectiveComponent* ObjectiveComponet)
{
	Objectives.Remove(ObjectiveComponet);
}

void UObjectiveWorldSubsystem::OnObjectiveStateChanged(UObjectiveComponent* ObjectiveComponent, EObjectiveState ObjectiveState)
{
	DisplayObjectiveWidget();
}