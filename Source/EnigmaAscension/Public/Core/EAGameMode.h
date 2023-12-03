// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/EAPlayerController.h"
#include "EAGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ENIGMAASCENSION_API AEAGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// Initialize values that are to be saved to players here
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	FTimerHandle ServerTime;

	UPROPERTY(EditAnywhere, Category = "Logs")
	bool bShowBufferContent = false;
	UPROPERTY(EditAnywhere, Category = "Logs")
	int InputBufferLogSize = 10;
	// Size of buffer 
	int BufferSize = 1000;

	// The amount of time that has passed since the game started in milliseconds.
	int ServerTimeInMs=0;
	
	// The current frame that the server is on, updated every tick
	int Server_FrameCount = 0;
	
	// Stores inputs of clients here, TArray because there is a possibility that multiple clients give inputs at the same frame
	TArray<FPlayerInputData> InputBuffer;
	
	// Stores the frame the server was on when the client joined the match
	TMap<int,int> ClientStartFrame;
	
	// Adds Inputs to the InputBuffer
	void AddInputToBuffer(const FPlayerInputData& Data);

	// Updates ServerTime and InputBuffer
	void UpdateInputBuffer();

	// Displays Contents of the Buffer Head in Logs
	void PrintBufferHead();
	
	// Displays Contents of the Buffer Tail in Logs
	void PrintBufferTail();
	
};

/*
Currently using difference between frames using the client and server and have capped the fps to 30 for both server and
all clients.
Not sure if this also counts as comparing server and client time will have to confirm
A better idea might be to start a timer to loop every 0.001 seconds and add slots to buffer in that. Then use player
ping to go back.
BUT this means that buffer size will increase even faster so could be too space expensive as compared to doing it by
frame which will update buffer every 0.033 seconds (for 30 FPS)

Initially thought of sending the target info too but that is calculated by the server so the client can't send that info
Also since that info is also only available when a few frames pass after the client send an input
If the rollback has to be done right after the input is received to the server, we wont be able to get this info at that
point


Another idea is to keep saving states at tick. So like at every frame (or every 1 ms if were going for that)
save the pos of all clients in the match. Then we the input to the server is received, it finds the frame/time in ms
it has to go back gets the player pos it saved from there, and finds out through some calculations if there was a target
and how much damage should be done etc.

Now to to calculate the hits and damage...
1 - Will need to find out the position of the socket is when the "anim notify" is called. (Manually store in var?)
2 - Also have to find out the exact time the anim notify is called. (Manually store in var?)
3 - Use that time to go forward to the frames after the input was pressed
4 - Get the locations of all clients at that time
5 - Check if the locations overlaps with the socket position + Radius of attack
6 - Apply damage to those actors.

Another question is what to do with the targets whose attacks happened first but the pressed input later.
Do i just cancel their attacks and rollback that too?


*/