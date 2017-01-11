// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "RBAIController.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API ARBAIController : public AAIController
{
	GENERATED_BODY()
	
	public: 
		/** Makes AI go toward specified destination */
		EPathFollowingRequestResult::Type MoveTo(const FAIMoveRequest& MoveRequest);
		

		/** Makes AI go toward specified Goal actor (destination will be continuously updated)
		*  @param AcceptanceRadius - finish move if pawn gets close enough
		*  @param bStopOnOverlap - add pawn's radius to AcceptanceRadius
		*  @param bUsePathfinding - use navigation data to calculate path (otherwise it will go in straight line)
		*  @param bCanStrafe - set focus related flag: bAllowStrafe
		*  @param FilterClass - navigation filter for pathfinding adjustments
		*  @param bAllowPartialPath - use incomplete path when goal can't be reached
		*	@note AcceptanceRadius has default value or -1 due to Header Parser not being able to recognize UPathFollowingComponent::DefaultAcceptanceRadius
		*/
		UFUNCTION(BlueprintCallable, Category = "AI|Navigation", Meta = (AdvancedDisplay = "bStopOnOverlap,bCanStrafe,bAllowPartialPath"))
			EPathFollowingRequestResult::Type MoveToActor(AActor* Goal, float AcceptanceRadius = -1, bool bStopOnOverlap = true,
				bool bUsePathfinding = true, bool bCanStrafe = true,
				TSubclassOf<UNavigationQueryFilter> FilterClass = NULL, bool bAllowPartialPath = true);

		/** Makes AI go toward specified Dest location
		*  @param AcceptanceRadius - finish move if pawn gets close enough
		*  @param bStopOnOverlap - add pawn's radius to AcceptanceRadius
		*  @param bUsePathfinding - use navigation data to calculate path (otherwise it will go in straight line)
		*  @param bProjectDestinationToNavigation - project location on navigation data before using it
		*  @param bCanStrafe - set focus related flag: bAllowStrafe
		*  @param FilterClass - navigation filter for pathfinding adjustments
		*  @param bAllowPartialPath - use incomplete path when goal can't be reached
		*	@note AcceptanceRadius has default value or -1 due to Header Parser not being able to recognize UPathFollowingComponent::DefaultAcceptanceRadius
		*/
		UFUNCTION(BlueprintCallable, Category = "AI|Navigation", Meta = (AdvancedDisplay = "bStopOnOverlap,bCanStrafe,bAllowPartialPath"))
			EPathFollowingRequestResult::Type MoveToLocation(const FVector& Dest, float AcceptanceRadius = -1, bool bStopOnOverlap = true,
				bool bUsePathfinding = true, bool bProjectDestinationToNavigation = false, bool bCanStrafe = true,
				TSubclassOf<UNavigationQueryFilter> FilterClass = NULL, bool bAllowPartialPath = true);
		
		FAIRequestID RequestPathAndMove(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query);
};
