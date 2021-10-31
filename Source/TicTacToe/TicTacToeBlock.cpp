// Copyright Epic Games, Inc. All Rights Reserved.

#include "TicTacToeBlock.h"
#include "TicTacToeBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

ATicTacToeBlock::ATicTacToeBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> P1Material;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> P2Material;

		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, P1Material(TEXT("/Game/Puzzle/Meshes/YellowMaterial.YellowMaterial"))
			, P2Material(TEXT("/Game/Puzzle/Meshes/RedMaterial.RedMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BaseMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &ATicTacToeBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &ATicTacToeBlock::OnFingerPressedBlock);

	// Save a pointer to necessary materials
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	P1Material = ConstructorStatics.P1Material.Get();
	P2Material = ConstructorStatics.P2Material.Get();

	// Initialize block active state
	isActive = false;

	// Initialize ownership
	p1Owned = false;
	p2Owned = false;
}

ATicTacToeBlock::~ATicTacToeBlock() { }

void ATicTacToeBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void ATicTacToeBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void ATicTacToeBlock::HandleClicked()
{
	// If game has completed, don't allow anymore moves
	if (OwningGrid->gameCompleted)
		return;

	// Check if block is not already active
	if (!isActive)
	{
		if (OwningGrid->isP1) // Is Player 1?
		{
			// Change to Player 1 material
			BlockMesh->SetMaterial(0, P1Material);
			p1Owned = true;
		}
		else if (OwningGrid->isP2) // Is Player 2?
		{
			// Change to Player 2 material
			BlockMesh->SetMaterial(0, P2Material);
			p2Owned = true;
		}

		isActive = true;

		// Tell the Grid
		if (OwningGrid != nullptr)
		{
			OwningGrid->SwitchPlayers();
			OwningGrid->DetermineWinner();
		}
	}
}

void ATicTacToeBlock::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated by a player or the game is over
	if (isActive || OwningGrid->gameCompleted)
		return;

	if (bOn) // if hovering over...
	{
		if (OwningGrid->isP1) // Is Player 1?
		{
			// Change to Player 1 Material for preview selection
			BlockMesh->SetMaterial(0, P1Material);
		}
		else if (OwningGrid->isP2) // Is Player 2?
		{
			// Change to Player 2 Material for preview selection
			BlockMesh->SetMaterial(0, P2Material);
		}
	}
	else // not hovering over...
	{
		// Return back to current material before hover
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
}

void ATicTacToeBlock::DispatchDestroy()
{
	Destroy();
}

void ATicTacToeBlock::DispatchMaterialChange(int32 index, UMaterialInterface* material)
{
	BlockMesh->SetMaterial(index, material);
}
