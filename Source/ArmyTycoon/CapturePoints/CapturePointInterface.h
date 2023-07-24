// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include "CapturePointInterface.generated.h"



UINTERFACE(MinimalAPI, Blueprintable)
class UCapturePointInterface : public UInterface
{
	GENERATED_BODY()
};

class ICapturePointInterface
{
	GENERATED_BODY()

public:

	virtual bool CanCapturePoint(class AActor* ToCapturePoint, int32 TeamID) const = 0;
	virtual void Capture(class AActor* ToCapturePoint, int32 TeamID) = 0;
	virtual void ReleaseCapture(class AActor* ToRealeseCapture) {};

	virtual AActor* GetPointOwner() const = 0;
};


UINTERFACE(MinimalAPI, Blueprintable)
class UCustomCaptureSpeedIncreaseInterface : public UInterface
{
	GENERATED_BODY()
};

class ICustomCaptureSpeedIncreaseInterface
{
	GENERATED_BODY()

public:

	virtual float GetCustomCaptureSpeedIncrease() const = 0;
};



UINTERFACE(MinimalAPI, Blueprintable)
class UCapturePointProgressInterface : public UCapturePointInterface
{
	GENERATED_BODY()
};

class ICapturePointProgressInterface : public ICapturePointInterface
{
	GENERATED_BODY()

public:

	virtual void StopCapture() = 0;

	virtual float GetCaptureProgress() const = 0;

	//virtual void AddCaptureProgress(float AddValue) = 0;
	//virtual void ReduceCaptureProgress(float ReduceValue) = 0;
};