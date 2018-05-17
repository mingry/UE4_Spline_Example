// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	

	// Set Key Values
	key_ps[0].Set(-100, 0, -1 - 0);	// for starting velocity.
	key_ps[1].Set(0, 0, 100);       // key 1
	key_ps[2].Set(150, 0, 300);     // key 2
	key_ps[3].Set(150, 500, 300);   // key 3
	key_ps[4].Set(0, 150, 200);     // key 4
	key_ps[5].Set(-150, 200, 500);  // key5
	key_ps[6].Set(-200, 300, 700);  // for ending velocity. 

	key_qs[0] = FQuat(FRotator(0, 0, 0));      // for starting velocity.
	key_qs[1] = FQuat(FRotator(0, 0, 0));      // key 1
	key_qs[2] = FQuat(FRotator(0, 45, 0));     // key 2
	key_qs[3] = FQuat(FRotator(0, 135, 45));   // key 3
	key_qs[4] = FQuat(FRotator(60, 45, 60));   // key 4
	key_qs[5] = FQuat(FRotator(90, 0, 0));     // key5
	key_qs[6] = FQuat(FRotator(60, 30, 0));    // for ending velocity. 

	for (int i = 1; i < num_keys; i++)
	{
		key_qs[i].EnforceShortestArcWith(key_qs[i - 1]);
	}

	key_ts[0] = -1;	// for starting velocity.
	key_ts[1] = 0;  // key 1
	key_ts[2] = 2;  // key 2
	key_ts[3] = 4;  // key 3
	key_ts[4] = 6;  // key 4
	key_ts[5] = 8;  // key5
	key_ts[6] = 9;  // for ending velocity. 


	// Draw for Debug
	// Key Points
	for (int i = 1; i < num_keys-1; i++)
	{
		DrawDebugBox(
			GetWorld(),
			key_ps[i],
			FVector(10, 10, 10),   // size
			key_qs[i],
			FColor(255, 0, 255),   // color
			true                   // persistent (never goes away)
		);
	}

	// Lines
	for (int i = 1; i < num_keys-2; i++)
	{
		DrawDebugLine(
			GetWorld(),
			key_ps[i],
			key_ps[i + 1],
			FColor(0, 0, 0),
			true
		);
	}
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	float gt = GetGameTimeSinceCreation();

	FVector result_p;
	FQuat result_q;

	if (gt < key_ts[1])	// if the current time (gt) is less than the first keytime (key_ts[1]).
	{
		result_p = key_ps[1];
		result_q = key_qs[1];
	}
	else if (gt > key_ts[num_keys-2]) // if the current time (gt) is greater than the last keytime (key_ts[num_keys-2]).
	{
		result_p = key_ps[num_keys-2];
		result_q = key_qs[num_keys-2];
	}
	else
	{
		// find k_i, a key index that is closest former to 'gt'.
		int k_i = 1;	
		for (int i = 1; i < num_keys - 2; i++)
		{
			if (gt < key_ts[i + 1])
			{
				k_i = i;
				break;
			}
		}

		// Catmull-Rom Interpolation
		result_p = 
			FMath::CubicCRSplineInterpSafe(
				key_ps[k_i-1], key_ps[k_i], key_ps[k_i + 1], key_ps[k_i + 2],
				key_ts[k_i-1], key_ts[k_i], key_ts[k_i+1], key_ts[k_i+2],
				gt
			);

		result_q =
			FMath::CubicCRSplineInterpSafe(
				key_qs[k_i - 1], key_qs[k_i], key_qs[k_i + 1], key_qs[k_i + 2],
				key_ts[k_i - 1], key_ts[k_i], key_ts[k_i + 1], key_ts[k_i + 2],
				gt
			);
	}


	SetActorLocationAndRotation(result_p, result_q);

}

