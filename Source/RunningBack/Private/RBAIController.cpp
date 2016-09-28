// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "RBAIController.h"
#include "VisualLogger.h"
#include "Stats2.h"
#include "Private/AIModulePrivate.h"

DEFINE_LOG_CATEGORY(LogAINavigation);

EPathFollowingRequestResult::Type ARBAIController::MoveToActor(AActor* Goal, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bCanStrafe, TSubclassOf<UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths)
{
	FAIMoveRequest MoveReq(Goal);
	MoveReq.SetUsePathfinding(bUsePathfinding);
	MoveReq.SetAllowPartialPath(bAllowPartialPaths);
	MoveReq.SetNavigationFilter(FilterClass);
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	MoveReq.SetStopOnOverlap(bStopOnOverlap);
	MoveReq.SetCanStrafe(bCanStrafe);

	return MoveTo(MoveReq);
}

EPathFollowingRequestResult::Type ARBAIController::MoveToLocation(const FVector& Dest, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bProjectDestinationToNavigation, bool bCanStrafe, TSubclassOf<UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("MoveToLocation"));

	FAIMoveRequest MoveReq(Dest);
	MoveReq.SetUsePathfinding(bUsePathfinding);
	MoveReq.SetAllowPartialPath(bAllowPartialPaths);
	MoveReq.SetProjectGoalLocation(bProjectDestinationToNavigation);
	MoveReq.SetNavigationFilter(FilterClass);
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	MoveReq.SetStopOnOverlap(bStopOnOverlap);
	MoveReq.SetCanStrafe(bCanStrafe);

	return MoveTo(MoveReq);
}

FAIRequestID ARBAIController::RequestPathAndMove(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query)
{
	FAIRequestID RequestID;

	UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(GetWorld());
	if (NavSys)
	{
		FPathFindingResult PathResult = NavSys->FindPathSync(Query);
		if (PathResult.Result != ENavigationQueryResult::Error)
		{
			if (PathResult.IsSuccessful() && PathResult.Path.IsValid())
			{
				if (MoveRequest.IsMoveToActorRequest())
				{
					PathResult.Path->SetGoalActorObservation(*MoveRequest.GetGoalActor(), 100.0f);
				}

				PathResult.Path->EnableRecalculationOnInvalidation(true);

				RequestID = RequestMove(MoveRequest, PathResult.Path);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trying to find path resulted in Error"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *GetNameSafe(MoveRequest.GetGoalActor()));
			UE_VLOG(this, LogAINavigation, Error, TEXT("Trying to find path to %s resulted in Error"), *GetNameSafe(MoveRequest.GetGoalActor()));
		}
	}

	return RequestID;
}

EPathFollowingRequestResult::Type ARBAIController::MoveTo(const FAIMoveRequest& MoveRequest)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("MoveTo Called"));
	UE_VLOG(this, LogAINavigation, Log, TEXT("MoveTo: %s"), *MoveRequest.ToString());

	if (MoveRequest.IsValid() == false)
	{
		UE_VLOG(this, LogAINavigation, Error, TEXT("MoveTo request failed due MoveRequest not being valid. Most probably desireg Goal Actor not longer exists"), *MoveRequest.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Move Request Invalid"));
		return EPathFollowingRequestResult::Failed;
	}

	EPathFollowingRequestResult::Type Result = EPathFollowingRequestResult::Failed;
	bool bCanRequestMove = true;
	bool bAlreadyAtGoal = false;

	if (!MoveRequest.IsMoveToActorRequest())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Not a MoveToActor Requst"));
		if (MoveRequest.GetGoalLocation().ContainsNaN() || FAISystem::IsValidLocation(MoveRequest.GetGoalLocation()) == false)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Destination Not Valid"));
			UE_VLOG(this, LogAINavigation, Error, TEXT("AAIController::MoveTo: Destination is not valid! Goal(%s)"), TEXT_AI_LOCATION(MoveRequest.GetGoalLocation()));
			bCanRequestMove = false;
		}

		// fail if projection to navigation is required but it failed
		if (bCanRequestMove && MoveRequest.IsProjectingGoal())
		{
			UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(GetWorld());
			const FNavAgentProperties& AgentProps = GetNavAgentPropertiesRef();
			FNavLocation ProjectedLocation;

			if (NavSys && !NavSys->ProjectPointToNavigation(MoveRequest.GetGoalLocation(), ProjectedLocation, AgentProps.GetExtent(), &AgentProps))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MoveTo failed to project destination location to navmesh"));
				UE_VLOG_LOCATION(this, LogAINavigation, Error, MoveRequest.GetGoalLocation(), 30.f, FColor::Red, TEXT("AAIController::MoveTo failed to project destination location to navmesh"));
				bCanRequestMove = false;
			}

			MoveRequest.UpdateGoalLocation(ProjectedLocation.Location);
		}

		bAlreadyAtGoal = bCanRequestMove && GetPathFollowingComponent() &&
			GetPathFollowingComponent()->HasReached(MoveRequest.GetGoalLocation(), MoveRequest.GetAcceptanceRadius(), !MoveRequest.CanStopOnOverlap());
	}
	else
	{
		bAlreadyAtGoal = bCanRequestMove && GetPathFollowingComponent() &&
			GetPathFollowingComponent()->HasReached(*MoveRequest.GetGoalActor(), MoveRequest.GetAcceptanceRadius(), !MoveRequest.CanStopOnOverlap());
	}

	if (bAlreadyAtGoal)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Already at goal"));
		UE_VLOG(this, LogAINavigation, Log, TEXT("MoveToActor: already at goal!"));

		if (GetPathFollowingComponent())
		{
			// make sure previous move request gets aborted
			GetPathFollowingComponent()->AbortMove(TEXT("Aborting move due to new move request finishing with AlreadyAtGoal"), FAIRequestID::AnyRequest);

			GetPathFollowingComponent()->SetLastMoveAtGoal(true);
		}

		OnMoveCompleted(FAIRequestID::CurrentRequest, EPathFollowingResult::Success);
		Result = EPathFollowingRequestResult::AlreadyAtGoal;
	}
	else if (bCanRequestMove)
	{
		FPathFindingQuery Query;
		const bool bValidQuery = PreparePathfinding(MoveRequest, Query);
		const FAIRequestID RequestID = bValidQuery ? RequestPathAndMove(MoveRequest, Query) : FAIRequestID::InvalidRequest;

		if (RequestID.IsValid())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Move Request Successful!"));
			bAllowStrafe = MoveRequest.CanStrafe();
			Result = EPathFollowingRequestResult::RequestSuccessful;
		} 
		else
		{
			if (!bValidQuery)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Query Not Valid"));
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Request ID Not Valid"));
		}
	}

	if (Result == EPathFollowingRequestResult::Failed)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Path Follow Request Failed"));
		if (GetPathFollowingComponent())
		{
			GetPathFollowingComponent()->SetLastMoveAtGoal(false);
		}

		OnMoveCompleted(FAIRequestID::InvalidRequest, EPathFollowingResult::Invalid);
	}

	return Result;
}
