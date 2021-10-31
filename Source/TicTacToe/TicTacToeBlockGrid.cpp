// Copyright Epic Games, Inc. All Rights Reserved.

#include "TicTacToeBlockGrid.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

ATicTacToeBlockGrid::ATicTacToeBlockGrid()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> WinMaterial;

		FConstructorStatics(): WinMaterial(TEXT("/Game/Puzzle/Meshes/WinMaterial.WinMaterial")) { }
	};
	static FConstructorStatics ConstructorStatics;

	// Enable tick for primary actor
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
	SetActorTickEnabled(true);
	SetTickableWhenPaused(false);

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	Player1Score = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScorePlayer1"));
	Player1Score->SetRelativeLocation(FVector(-430.f, 649.f, 0.f));
	Player1Score->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	Player1Score->SetText(FText::Format(LOCTEXT("ScoreFmt", "P1 Wins: {0}"), FText::AsNumber(0)));
	Player1Score->SetupAttachment(DummyRoot);

	Player2Score = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScorePlayer2"));
	Player2Score->SetRelativeLocation(FVector(-360.f, 649.f, 0.f));
	Player2Score->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	Player2Score->SetText(FText::Format(LOCTEXT("ScoreFmt", "P2 Wins: {0}"), FText::AsNumber(0)));
	Player2Score->SetupAttachment(DummyRoot);

	// Save a pointer to necessary materials
	WinMaterial = ConstructorStatics.WinMaterial.Get();

	// Set defaults
	Size = 5;
	BlockSpacing = 300.f;
	totalBlocks = Size * Size;
	destroyDelay = 2.f;
	spawnDelay = 3.f;

	// Initialize players
	isP1 = true;
	isP2 = false;

	// Initialize game state
	gameCompleted = false;
	endGame = false;
}

ATicTacToeBlockGrid::~ATicTacToeBlockGrid() { }

void ATicTacToeBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	destroyDelegate.BindUFunction(this, FName("OnTimerDestroy"));
	spawnDelegate.BindUFunction(this, FName("OnTimerSpawn"));
	SpawnBlocks();
}

void ATicTacToeBlockGrid::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	// A winner was found, clear blocks and spawn new ones.
	if (endGame)
	{
		GetWorldTimerManager().SetTimer(destroyTimerHandle, destroyDelegate, destroyDelay, false);
		GetWorldTimerManager().SetTimer(spawnTimerHandle, spawnDelegate, spawnDelay, false);
		endGame = false;
	}
}


void ATicTacToeBlockGrid::AddScore(int32 playerPosition)
{
	switch (playerPosition)
	{
	case 1:
		// Increment score
		Player1Wins++;
		// Update text
		Player1Score->SetText(FText::Format(LOCTEXT("ScoreFmt", "P1 Wins: {0}"), FText::AsNumber(Player1Wins)));
		break;
	case 2:
		// Increment score
		Player2Wins++;
		// Update text
		Player2Score->SetText(FText::Format(LOCTEXT("ScoreFmt", "P2 Wins: {0}"), FText::AsNumber(Player2Wins)));
		break;
	}
}

void ATicTacToeBlockGrid::SwitchPlayers()
{
	isP1 = !isP1;
	isP2 = !isP2;
}

void ATicTacToeBlockGrid::SwitchPlayersByWin(int32 playerPosition)
{
	switch (playerPosition)
	{
	case 1:
		isP1 = true;
		isP2 = false;
		break;
	case 2:
		isP2 = true;
		isP1 = false;
	}
}

bool ATicTacToeBlockGrid::Player1WinCheck()
{
	//** NOTE: Look into a better way for automation of win conditions based on grid size **//

	//** Horizontal Conditions **//
	if ((blocksOnGrid[20]->p1Owned && blocksOnGrid[21]->p1Owned && blocksOnGrid[22]->p1Owned))
	{
		blocksOnGrid[20]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[21]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[22]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[22]->p1Owned && blocksOnGrid[23]->p1Owned && blocksOnGrid[24]->p1Owned))
	{
		blocksOnGrid[22]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[23]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[24]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[21]->p1Owned && blocksOnGrid[22]->p1Owned && blocksOnGrid[23]->p1Owned))
	{
		blocksOnGrid[21]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[22]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[23]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[15]->p1Owned && blocksOnGrid[16]->p1Owned && blocksOnGrid[17]->p1Owned))
	{
		blocksOnGrid[15]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[16]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[17]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[17]->p1Owned && blocksOnGrid[18]->p1Owned && blocksOnGrid[19]->p1Owned))
	{
		blocksOnGrid[17]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[18]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[19]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[16]->p1Owned && blocksOnGrid[17]->p1Owned && blocksOnGrid[18]->p1Owned))
	{
		blocksOnGrid[16]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[17]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[18]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[10]->p1Owned && blocksOnGrid[11]->p1Owned && blocksOnGrid[12]->p1Owned))
	{
		blocksOnGrid[10]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[11]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[12]->p1Owned && blocksOnGrid[13]->p1Owned && blocksOnGrid[14]->p1Owned))
	{
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[13]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[14]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[11]->p1Owned && blocksOnGrid[12]->p1Owned && blocksOnGrid[13]->p1Owned))
	{
		blocksOnGrid[11]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[13]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[5]->p1Owned && blocksOnGrid[6]->p1Owned && blocksOnGrid[7]->p1Owned))
	{
		blocksOnGrid[5]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[6]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[7]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[7]->p1Owned && blocksOnGrid[8]->p1Owned && blocksOnGrid[9]->p1Owned))
	{
		blocksOnGrid[7]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[8]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[9]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[6]->p1Owned && blocksOnGrid[7]->p1Owned && blocksOnGrid[8]->p1Owned))
	{
		blocksOnGrid[6]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[7]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[8]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[0]->p1Owned && blocksOnGrid[1]->p1Owned && blocksOnGrid[2]->p1Owned))
	{
		blocksOnGrid[0]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[1]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[2]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[2]->p1Owned && blocksOnGrid[3]->p1Owned && blocksOnGrid[4]->p1Owned))
	{
		blocksOnGrid[2]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[3]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[4]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[1]->p1Owned && blocksOnGrid[2]->p1Owned && blocksOnGrid[3]->p1Owned))
	{
		blocksOnGrid[1]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[2]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[3]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	//** Vertical Conditions **//
	else if ((blocksOnGrid[20]->p1Owned && blocksOnGrid[15]->p1Owned && blocksOnGrid[10]->p1Owned))
	{
		blocksOnGrid[20]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[15]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[10]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[10]->p1Owned && blocksOnGrid[5]->p1Owned && blocksOnGrid[0]->p1Owned))
	{
		blocksOnGrid[10]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[5]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[0]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[15]->p1Owned && blocksOnGrid[10]->p1Owned && blocksOnGrid[5]->p1Owned))
	{
		blocksOnGrid[15]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[10]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[5]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[21]->p1Owned && blocksOnGrid[16]->p1Owned && blocksOnGrid[11]->p1Owned))
	{
		blocksOnGrid[21]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[16]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[11]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[11]->p1Owned && blocksOnGrid[6]->p1Owned && blocksOnGrid[1]->p1Owned))
	{
		blocksOnGrid[11]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[6]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[1]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[16]->p1Owned && blocksOnGrid[11]->p1Owned && blocksOnGrid[6]->p1Owned))
	{
		blocksOnGrid[16]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[11]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[6]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[22]->p1Owned && blocksOnGrid[17]->p1Owned && blocksOnGrid[12]->p1Owned))
	{
		blocksOnGrid[22]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[17]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[12]->p1Owned && blocksOnGrid[7]->p1Owned && blocksOnGrid[2]->p1Owned))
	{
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[7]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[2]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[17]->p1Owned && blocksOnGrid[12]->p1Owned && blocksOnGrid[7]->p1Owned))
	{
		blocksOnGrid[17]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[7]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[23]->p1Owned && blocksOnGrid[18]->p1Owned && blocksOnGrid[13]->p1Owned))
	{
		blocksOnGrid[23]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[18]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[13]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[13]->p1Owned && blocksOnGrid[8]->p1Owned && blocksOnGrid[3]->p1Owned))
	{
		blocksOnGrid[13]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[8]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[3]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[18]->p1Owned && blocksOnGrid[13]->p1Owned && blocksOnGrid[8]->p1Owned))
	{
		blocksOnGrid[18]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[13]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[8]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[24]->p1Owned && blocksOnGrid[19]->p1Owned && blocksOnGrid[14]->p1Owned))
	{
		blocksOnGrid[24]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[19]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[14]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[14]->p1Owned && blocksOnGrid[9]->p1Owned && blocksOnGrid[4]->p1Owned))
	{
		blocksOnGrid[14]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[9]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[4]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[19]->p1Owned && blocksOnGrid[14]->p1Owned && blocksOnGrid[9]->p1Owned))
	{
		blocksOnGrid[19]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[14]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[9]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	//** Diagonal Conditions **//
	else if ((blocksOnGrid[20]->p1Owned && blocksOnGrid[16]->p1Owned && blocksOnGrid[12]->p1Owned))
	{
		blocksOnGrid[20]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[16]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[12]->p1Owned && blocksOnGrid[8]->p1Owned && blocksOnGrid[4]->p1Owned))
	{
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[8]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[4]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[16]->p1Owned && blocksOnGrid[12]->p1Owned && blocksOnGrid[8]->p1Owned))
	{
		blocksOnGrid[16]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[8]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[24]->p1Owned && blocksOnGrid[18]->p1Owned && blocksOnGrid[12]->p1Owned))
	{
		blocksOnGrid[24]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[18]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[12]->p1Owned && blocksOnGrid[6]->p1Owned && blocksOnGrid[0]->p1Owned))
	{
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[6]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[0]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[18]->p1Owned && blocksOnGrid[12]->p1Owned && blocksOnGrid[6]->p1Owned))
	{
		blocksOnGrid[18]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[6]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Yellow, "Player 1 Wins!");
		AddScore(1);
		SwitchPlayersByWin(1);
		gameCompleted = endGame = true;
		return true;
	}

	return false;
}

bool ATicTacToeBlockGrid::Player2WinCheck()
{
	//** NOTE: Look into a better way for automation of win conditions based on grid size **//

	//** Horizontal Conditions **//
	if ((blocksOnGrid[20]->p2Owned && blocksOnGrid[21]->p2Owned && blocksOnGrid[22]->p2Owned))
	{
		blocksOnGrid[20]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[21]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[22]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[22]->p2Owned && blocksOnGrid[23]->p2Owned && blocksOnGrid[24]->p2Owned))
	{
		blocksOnGrid[22]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[23]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[24]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[21]->p2Owned && blocksOnGrid[22]->p2Owned && blocksOnGrid[23]->p2Owned))
	{
		blocksOnGrid[21]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[22]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[23]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[15]->p2Owned && blocksOnGrid[16]->p2Owned && blocksOnGrid[17]->p2Owned))
	{
		blocksOnGrid[15]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[16]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[17]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[17]->p2Owned && blocksOnGrid[18]->p2Owned && blocksOnGrid[19]->p2Owned))
	{
		blocksOnGrid[17]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[18]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[19]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[16]->p2Owned && blocksOnGrid[17]->p2Owned && blocksOnGrid[18]->p2Owned))
	{
		blocksOnGrid[16]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[17]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[18]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[10]->p2Owned && blocksOnGrid[11]->p2Owned && blocksOnGrid[12]->p2Owned))
	{
		blocksOnGrid[10]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[11]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[12]->p2Owned && blocksOnGrid[13]->p2Owned && blocksOnGrid[14]->p2Owned))
	{
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[13]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[14]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[11]->p2Owned && blocksOnGrid[12]->p2Owned && blocksOnGrid[13]->p2Owned))
	{
		blocksOnGrid[11]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[13]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[5]->p2Owned && blocksOnGrid[6]->p2Owned && blocksOnGrid[7]->p2Owned))
	{
		blocksOnGrid[5]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[6]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[7]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[7]->p2Owned && blocksOnGrid[8]->p2Owned && blocksOnGrid[9]->p2Owned))
	{
		blocksOnGrid[7]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[8]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[9]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[6]->p2Owned && blocksOnGrid[7]->p2Owned && blocksOnGrid[8]->p2Owned))
	{
		blocksOnGrid[6]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[7]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[8]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[0]->p2Owned && blocksOnGrid[1]->p2Owned && blocksOnGrid[2]->p2Owned))
	{
		blocksOnGrid[0]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[1]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[2]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[2]->p2Owned && blocksOnGrid[3]->p2Owned && blocksOnGrid[4]->p2Owned))
	{
		blocksOnGrid[2]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[3]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[4]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[1]->p2Owned && blocksOnGrid[2]->p2Owned && blocksOnGrid[3]->p2Owned))
	{
		blocksOnGrid[1]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[2]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[3]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	//** Vertical Conditions **//
	else if ((blocksOnGrid[20]->p2Owned && blocksOnGrid[15]->p2Owned && blocksOnGrid[10]->p2Owned))
	{
		blocksOnGrid[20]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[15]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[10]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[10]->p2Owned && blocksOnGrid[5]->p2Owned && blocksOnGrid[0]->p2Owned))
	{
		blocksOnGrid[10]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[5]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[0]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[15]->p2Owned && blocksOnGrid[10]->p2Owned && blocksOnGrid[5]->p2Owned))
	{
		blocksOnGrid[15]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[10]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[5]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[21]->p2Owned && blocksOnGrid[16]->p2Owned && blocksOnGrid[11]->p2Owned))
	{
		blocksOnGrid[21]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[16]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[11]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[11]->p2Owned && blocksOnGrid[6]->p2Owned && blocksOnGrid[1]->p2Owned))
	{
		blocksOnGrid[11]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[6]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[1]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[16]->p2Owned && blocksOnGrid[11]->p2Owned && blocksOnGrid[6]->p2Owned))
	{
		blocksOnGrid[16]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[11]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[6]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[22]->p2Owned && blocksOnGrid[17]->p2Owned && blocksOnGrid[12]->p2Owned))
	{
		blocksOnGrid[22]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[17]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[12]->p2Owned && blocksOnGrid[7]->p2Owned && blocksOnGrid[2]->p2Owned))
	{
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[7]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[2]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[17]->p2Owned && blocksOnGrid[12]->p2Owned && blocksOnGrid[7]->p2Owned))
	{
		blocksOnGrid[17]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[7]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[23]->p2Owned && blocksOnGrid[18]->p2Owned && blocksOnGrid[13]->p2Owned))
	{
		blocksOnGrid[23]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[18]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[13]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[13]->p2Owned && blocksOnGrid[8]->p2Owned && blocksOnGrid[3]->p2Owned))
	{
		blocksOnGrid[13]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[8]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[3]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[18]->p2Owned && blocksOnGrid[13]->p2Owned && blocksOnGrid[8]->p2Owned))
	{
		blocksOnGrid[18]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[13]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[8]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[24]->p2Owned && blocksOnGrid[19]->p2Owned && blocksOnGrid[14]->p2Owned))
	{
		blocksOnGrid[24]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[19]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[14]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[14]->p2Owned && blocksOnGrid[9]->p2Owned && blocksOnGrid[4]->p2Owned))
	{
		blocksOnGrid[14]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[9]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[4]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[19]->p2Owned && blocksOnGrid[14]->p2Owned && blocksOnGrid[9]->p2Owned))
	{
		blocksOnGrid[19]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[14]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[9]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	//** Diagonal Conditions **//
	else if ((blocksOnGrid[20]->p2Owned && blocksOnGrid[16]->p2Owned && blocksOnGrid[12]->p2Owned))
	{
		blocksOnGrid[20]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[16]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[12]->p2Owned && blocksOnGrid[8]->p2Owned && blocksOnGrid[4]->p2Owned))
	{
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[8]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[4]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[16]->p2Owned && blocksOnGrid[12]->p2Owned && blocksOnGrid[8]->p2Owned))
	{
		blocksOnGrid[16]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[8]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[24]->p2Owned && blocksOnGrid[18]->p2Owned && blocksOnGrid[12]->p2Owned))
	{
		blocksOnGrid[24]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[18]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[12]->p2Owned && blocksOnGrid[6]->p2Owned && blocksOnGrid[0]->p2Owned))
	{
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[6]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[0]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}
	else if ((blocksOnGrid[18]->p2Owned && blocksOnGrid[12]->p2Owned && blocksOnGrid[6]->p2Owned))
	{
		blocksOnGrid[18]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[12]->DispatchMaterialChange(0, WinMaterial);
		blocksOnGrid[6]->DispatchMaterialChange(0, WinMaterial);

		DebugMessage(FColor::Red, "Player 2 Wins!");
		AddScore(2);
		SwitchPlayersByWin(2);
		gameCompleted = endGame = true;
		return true;
	}

	return false;
}

void ATicTacToeBlockGrid::DrawCheck()
{
	//** Draw Condition Check **//
	int32 blocksNotOwned = 0;
	for (int32 BlockIndex = 0; BlockIndex < totalBlocks; BlockIndex++)
	{
		// If there is a block without either player 1 or player 2 ownership, then there is still a move to choose
		if (!blocksOnGrid[BlockIndex]->p1Owned && !blocksOnGrid[BlockIndex]->p2Owned)
		{
			blocksNotOwned++;
		}
	}

	// Draw condition met!
	if (blocksNotOwned == 0)
	{
		DebugMessage(FColor::Green, "Draw!");
		gameCompleted = endGame = true;
	}
}

void ATicTacToeBlockGrid::DetermineWinner()
{
	if (Player1WinCheck())
		return;

	if (Player2WinCheck())
		return;

	DrawCheck();
}

void ATicTacToeBlockGrid::DebugMessage(FColor color, FString message)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, color, message);
}

void ATicTacToeBlockGrid::OnTimerDestroy()
{
	RemoveBlocks();
}

void ATicTacToeBlockGrid::OnTimerSpawn()
{
	SpawnBlocks();
	gameCompleted = false;
}

void ATicTacToeBlockGrid::SpawnBlocks()
{
	// Loop to spawn each block
	for (int32 BlockIndex = 0; BlockIndex < totalBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex / Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex % Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		ATicTacToeBlock* NewBlock = GetWorld()->SpawnActor<ATicTacToeBlock>(BlockLocation, FRotator(0, 0, 0));

		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
			blocksOnGrid.Insert(NewBlock, BlockIndex);
		}
	}
}

void ATicTacToeBlockGrid::RemoveBlocks()
{
	// Destroy all blocks
	for (int32 BlockIndex = 0; BlockIndex < totalBlocks; BlockIndex++)
	{
		blocksOnGrid[BlockIndex]->DispatchDestroy();
	}

	// Empty blocks saved in TAarray
	blocksOnGrid.Empty();
}

#undef LOCTEXT_NAMESPACE
