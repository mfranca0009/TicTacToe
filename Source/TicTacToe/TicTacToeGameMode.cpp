// Copyright Epic Games, Inc. All Rights Reserved.

#include "TicTacToeGameMode.h"
#include "TicTacToePlayerController.h"
#include "TicTacToePawn.h"

ATicTacToeGameMode::ATicTacToeGameMode()
{
	// no pawn by default
	DefaultPawnClass = ATicTacToePawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ATicTacToePlayerController::StaticClass();
}
