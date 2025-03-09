// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/GBPathfinding.h"
#include "Grid/GBTileBase.h"
// #include "Player/TBPlayerController.h"

/* Sets default values
UPathfinding::UPathfinding()
{

}*/

TArray<AGBTileBase*> UGBPathfinding::FindPath(AGBTileBase* startNode, AGBTileBase* targetNode)
{
	if (!IsValid(startNode) || !IsValid(targetNode)) {
		UE_LOG(LogTemp, Warning, TEXT("Pathfinding called while no tiles set. startNode: %s targetNode: %s"), *startNode->GetName(), *targetNode->GetName());
		return TArray<AGBTileBase*>();
	}

	TArray<AGBTileBase*> toSearch;
	toSearch.Add(startNode);
	TArray<AGBTileBase*> processed;

	while (!toSearch.IsEmpty())
	{
		/*
		THIS MAY LATER BE DELETED because of movement changes on players. However could still be useful for queue player movement or enemy AI calculation of player location.
		OPTIMIZE THIS
		Currently finding best F cost node is the best way to optimize pathfinding
		Even though it is probably not necessary for project this big,
		but would look better if project is sent a part of portfolio and also for learning optimization
		
		Could be replaced with a Heap

		*/
		AGBTileBase* current = toSearch[0];
		for (AGBTileBase* node : toSearch)
		{
			if (node->F < current->F || node->F == current->F && node->H < current->H)
				current = node;
		}
		
			processed.Add(current);
			toSearch.Remove(current);
		

		if (current == targetNode)
		{
			AGBTileBase* currentPathTile = targetNode;
			TArray<AGBTileBase*> path;
			int count = 100;
			while (currentPathTile != startNode)
			{
				path.Add(currentPathTile);
				currentPathTile = currentPathTile->Connection;
				count--;
				if (count < 0) throw;
			}
			return path;
		}

		//Process walkable and non-processed tiles here
		for (AGBTileBase* neighbor : current->Neighbors)
		{
			if ( neighbor->Walkable == true && !processed.Contains(neighbor))
			{
				bool inSearch = toSearch.Contains(neighbor);

				float costToNeighbor = current->G + current->GetDistance(neighbor);

				if (!inSearch || costToNeighbor < neighbor->G)
				{
					neighbor->SetG(costToNeighbor);

					//TODO: Check this functions necessity
					neighbor->SetConnection(current);

					if (!inSearch)
					{
						neighbor->SetH(neighbor->GetDistance(targetNode));
						toSearch.Add(neighbor);
					}
				}
			}
		}
	}
	return TArray<AGBTileBase*>();
}