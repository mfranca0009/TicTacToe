// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacToeBlock.generated.h"

/** A block that can be clicked */
UCLASS(minimalapi)
class ATicTacToeBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

public:
	ATicTacToeBlock();
	~ATicTacToeBlock();

	/** Are we currently active? */
	bool isActive;

	/** Who currently owns the block? */
	bool p1Owned;
	bool p2Owned;

	/** Pointer to base material */
	UPROPERTY()
	class UMaterial* BaseMaterial;

	/** Pointer to Player 1 Material Instance */
	UPROPERTY()
	class UMaterialInstance* P1Material;

	/** Pointer to Player 2 Material Instance */
	UPROPERTY()
	class UMaterialInstance* P2Material;

	/** Grid that owns us */
	UPROPERTY()
	class ATicTacToeBlockGrid* OwningGrid;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void HandleClicked();

	void Highlight(bool bOn);

	/** Handles destroy of block */
	void DispatchDestroy();

	/** Handles change to win material */
	void DispatchMaterialChange(int32 index, UMaterialInterface* material);

public:

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }

};