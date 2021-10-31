// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacToeBlock.h"
#include "TicTacToeBlockGrid.generated.h"

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class ATicTacToeBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for Player 1 Score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* Player1Score;

	/** Text component for Player 2 Score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTextRenderComponent* Player2Score;

public:
	ATicTacToeBlockGrid();
	~ATicTacToeBlockGrid();

	/** Player 1 Win Count */
	int32 Player1Wins;

	/** Player 2 Win Count */
	int32 Player2Wins;

	/** Are we Player 1 or Player2? */
	bool isP1;
	bool isP2;

	/** Does game need to end? */
	bool endGame;

	/** Has game completed? */
	bool gameCompleted;

	/** Pointer to Win Material Instance */
	UPROPERTY()
	class UMaterialInstance* WinMaterial;

	/** Number of blocks along each side of grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

	/** Block destroy time delay */
	UPROPERTY(Category=Timers, EditAnywhere, BlueprintReadOnly)
	float destroyDelay;

	/** Block spawn time delay */
	UPROPERTY(Category = Timers, EditAnywhere, BlueprintReadOnly)
	float spawnDelay;

private:

	/** Total blocks on grid */
	int32 totalBlocks;

	/** Tracking blocks on grid by TArray */
	TArray<ATicTacToeBlock*> blocksOnGrid;

	FTimerDelegate destroyDelegate;
	/** Handle for block destroy timer */
	FTimerHandle destroyTimerHandle;

	FTimerDelegate spawnDelegate;
	/** Handle for block spawn timer */
	FTimerHandle spawnTimerHandle;

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;

	virtual void Tick(float deltaSeconds) override;
	// End AActor interface

public:

	/** Handle Adding Score to Win Tracker */
	void AddScore(int32 playerPosition);

	/** Handle the switch between player turns */
	void SwitchPlayers();

	/** Handle the switch between player turns by whomever wins the last game */
	void SwitchPlayersByWin(int32 playerPosition);

	/** Check if player 1 won the game */
	bool Player1WinCheck();

	/** Check if player 2 won the game */
	bool Player2WinCheck();

	/** Check if the game has ended in a draw */
	void DrawCheck();

	/** Handle when a player wins */
	void DetermineWinner();

	/** Handle debug message output */
	void DebugMessage(FColor color, FString message);

	UFUNCTION()
		void OnTimerDestroy();

	UFUNCTION()
		void OnTimerSpawn();

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

	/** Returns Player1 Score Text subobject **/
	FORCEINLINE class UTextRenderComponent* GetPlayer1ScoreText() const { return Player1Score; }

	/** Returns Player2 Score Text suboject **/
	FORCEINLINE class UTextRenderComponent* GetPlayer2ScoreText() const { return Player2Score; }

private:

	/** Handles creation of blocks within the grid */
	void SpawnBlocks();

	/** Handles Deletion of blocks within the grid */
	void RemoveBlocks();
};



