// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "RunningBack.h"
#include "RunningBackWheelFront.h"

URunningBackWheelFront::URunningBackWheelFront()
{
	ShapeRadius = 35.f;
	ShapeWidth = 10.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 50.f;
}
