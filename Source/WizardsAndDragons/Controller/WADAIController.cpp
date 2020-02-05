// Fill out your copyright notice in the Description page of Project Settings.
#include "WADAIController.h"
#include "../AI/AIDragon.h"
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "../Player/WizardsAndDragonsCharacter.h"

AWADAIController::AWADAIController(const FObjectInitializer& ObjectInitializer)
{
	PerceptionComponent = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("PerceptionComponent"));
	PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AWADAIController::OnActorPerceptionUpdated);
}

void AWADAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 Index = Targets.Num() - 1; Index >= 0; --Index)
	{
		FAITargetInfo& TargetInfo = Targets[Index];

		if (TargetInfo.Aggro < 0.0f && !TargetInfo.bVisible)
		{
			Targets.RemoveAt(Index);
		}
		else if (!TargetInfo.bVisible)
		{
			TargetInfo.Aggro -= DeltaTime;
		}
	}
}

void AWADAIController::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	AAIDragon* AIDragon = GetAIDragon();

	if (AIDragon == nullptr)
	{
		Super::GetActorEyesViewPoint(out_Location, out_Rotation);
	}
	else
	{
		out_Location = AIDragon->GetCapsuleComponent()->GetComponentLocation();
		out_Rotation = AIDragon->GetActorRotation();
	}
}

void AWADAIController::OnActorPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!HasTarget(Actor))
	{
			AddNewTarget(Actor, Stimulus.WasSuccessfullySensed());	
	}
	else
	{
		UpdateTarget(Actor, Stimulus.WasSuccessfullySensed());
	}
}

ETeamAttitude::Type AWADAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	ETeamAttitude::Type Attitude = ETeamAttitude::Hostile;

	const AAIDragon* MyCharacter = GetAIDragon();
	const AAIDragon* OtherCharacter = Cast<AAIDragon>(&Other);

	if (MyCharacter && OtherCharacter)
	{
		if (MyCharacter->Team == OtherCharacter->Team)
		{
			Attitude = ETeamAttitude::Friendly;
		}
	}

	return Attitude;
}

class AAIDragon* AWADAIController::GetAIDragon() const
{
	return CastChecked<AAIDragon>(GetPawn(), ECastCheckedType::NullAllowed);
}

bool AWADAIController::GetTargetWithHighestAggro(FAITargetInfo& OutTargetInfo) const
{
	OutTargetInfo = FAITargetInfo();

	float HighestAggro = 0.0f;
	bool bFoundTarget = false;

	for (const FAITargetInfo& TargetInfo : Targets)
	{
		if (TargetInfo.Aggro > HighestAggro)
		{
			HighestAggro = TargetInfo.Aggro;
			OutTargetInfo = TargetInfo;
		}
	}

	return bFoundTarget;
}

TArray<FAITargetInfo> AWADAIController::GetAllVisibleTargets() const
{
	TArray<FAITargetInfo> Out;

	for (const FAITargetInfo& TargetInfo : Targets)
	{
		if (TargetInfo.bVisible)
		{
			Out.Add(TargetInfo);
		}
	}
	return Out;
}

bool AWADAIController::HasTarget(AActor* TargetActor) const
{
	for (const FAITargetInfo& TargetInfo : Targets)
	{
		if (TargetInfo.TargetActor == TargetActor)
		{
			return true;
		}
	}

	return false;
}

void AWADAIController::AddNewTarget(AActor* NewTarget, bool bVisible)
{
	FAITargetInfo TargetInfo;
	TargetInfo.TargetActor = NewTarget;
	TargetInfo.Aggro = 50.0f;
	TargetInfo.bVisible = bVisible;
	Targets.Add(TargetInfo);
}


void AWADAIController::AddAggro(AActor* TargetActor, float Aggro)
{
	if (!HasTarget(TargetActor))
	{
		return;
	}

	for (FAITargetInfo& TargetInfo : Targets)
	{
		if (TargetInfo.TargetActor == TargetActor)
		{
			TargetInfo.Aggro += Aggro;
		}
	}
}

void AWADAIController::UpdateTarget(AActor* TargetActor, bool bVisible)
{
	for (FAITargetInfo& TargetInfo : Targets)
	{
		if (TargetInfo.TargetActor == TargetActor)
		{
			TargetInfo.bVisible = bVisible;
		}
	}
}
