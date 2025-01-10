#include "AStarPathfinder.hpp"
#include <set>
#include <queue>
#include <iostream>
#include <cmath>
#include <Auxiliary/ConsoleHandler.hpp>
#include "../../../ACECS/ECSRegistry.hpp"


AStarPath AStarPathfinder::pathRetrace(AStarCell& cellStart, AStarCell& cellEnd) {
	
	AStarPath path;
	// current cell in retrace, used for finding path
	AStarCell* cellBackTraceCur = &cellEnd;

	while (*cellBackTraceCur != cellStart) {
		path.push_back(cellBackTraceCur->cellPositionWorld);

		if (cellBackTraceCur->cellParent == nullptr) {
			ConsoleHandler::consolePrintErr("Pathfinder pathRetrace failed: Exception: \"cellParent\" was nullptr");
			break;
		}
		else {
			cellBackTraceCur = cellBackTraceCur->cellParent;
		}
	}

	return path;
}
AStarPath AStarPathfinder::pathRetraceCellCoordinates(AStarCell& cellStart, AStarCell& cellEnd) {

	AStarPath path;
	// current cell in retrace, used for finding path
	AStarCell* cellBackTraceCur = &cellEnd;

	while (*cellBackTraceCur != cellStart) {
		path.push_back(sf::Vector2f(cellBackTraceCur->cellPositionGrid));

		if (cellBackTraceCur->cellParent == nullptr) {
			ConsoleHandler::consolePrintErr("Pathfinder pathRetrace failed: Exception: \"cellParent\" was nullptr");
			break;
		}
		else {
			cellBackTraceCur = cellBackTraceCur->cellParent;
		}
	}

	return path;
}

AStarPath AStarPathfinder::pathSimplify(AStarPath path) {
	// path has only two points, return path, since it can't be simplified
	if (path.size() <= 2) return path;
	
	std::reverse(path.begin(), path.end());

	AStarPath pathSimplified;

	std::vector<uint16_t> toRemoveIndices;
	
	sf::Vector2f dirStart;

	for (uint16_t i = 0; i < path.size()-1; i++) {

		sf::Vector2f dir = Vector2fMath::dir(path[i], path[i + 1]);

		if (dir != dirStart) {
			pathSimplified.push_back(path[i]);
			dirStart = dir;
		}
	}
	// push_back last element of path because it is skipped in the previous iteration
	pathSimplified.push_back(path.back());

	std::reverse(pathSimplified.begin(), pathSimplified.end());
	return pathSimplified;
}


void AStarPathfinder::cellsResetData(std::vector<AStarCell*> cells) {
	// reset costs of all cells
	for (uint16_t i = 0; i < cells.size(); i++) {
		cells[i]->resetPathingData();
	}
}

AStarCostValue AStarPathfinder::cellDistanceGet(AStarCellPosition cellPosFrom, AStarCellPosition cellPosTo) {
	AStarCostValue distX = AStarCostValue(std::abs(int(cellPosFrom.x) - int(cellPosTo.x)));
	AStarCostValue distY = AStarCostValue(std::abs(int(cellPosFrom.y) - int(cellPosTo.y)));

	if (distX > distY) {
		return (14 * distY) + 10 * (distX - distY);
	}
	else {
		return (14 * distX) + 10 * (distY - distX);
	}
}

struct AStarCellComparator {
	bool operator() (AStarCell* a, AStarCell* b) {
		return *a > *b;
	}
};
struct CellHeap : std::priority_queue<AStarCell*, std::vector<AStarCell*>, AStarCellComparator> {
public:

	bool has(const AStarCell& val) const {
		auto first = this->c.cbegin();
		auto last = this->c.cend();
		while (first != last) {
			if (**first == val) return true;
			first++;
		}
		return false;
	}
};

AStarPath AStarPathfinder::pathGetFromCellCoordinates(sf::Vector2u pointStart, sf::Vector2u pointEnd, AStarGrid& aStarGrid) {

	AStarCellPosition cellStart = pointStart;
	AStarCellPosition cellEnd = pointEnd;

	if (!aStarGrid.cellGet(cellStart).valid || !aStarGrid.cellGet(cellEnd).valid) {
		return AStarPath{ sf::Vector2f(pointStart) };
	}

	CellHeap cellsOpenHeap; // cells to be evaluated
	std::vector<AStarCellPosition> cellsClosedVector; // cells that have already been evaluated

	cellsOpenHeap.push(&aStarGrid.cellGet(cellStart));

	AStarCell* cellCurrent = cellsOpenHeap.top();

	while ((cellCurrent->cellPositionGrid != cellEnd) && (cellsOpenHeap.size() > 0)) {

		cellCurrent = cellsOpenHeap.top();
		cellsOpenHeap.pop();
		cellsClosedVector.push_back(cellCurrent->cellPositionGrid);

		for (int16_t x = -1; x <= 1; x++) {
			for (int16_t y = -1; y <= 1; y++) {

				if (x == 0 && y == 0) continue;

				int16_t cellPosOffsetX = int16_t(cellCurrent->cellPositionGrid.x + x);
				int16_t cellPosOffsetY = int16_t(cellCurrent->cellPositionGrid.y + y);

				if (!aStarGrid.cellPosIsInGrid(cellPosOffsetX, cellPosOffsetY)) continue;

				// if cell is a diagonal, make sure the pathfinder doesn't cut a corner
				if (x != 0 && y != 0) {
					bool isValidX = aStarGrid.cellGet(cellPosOffsetX, cellCurrent->cellPositionGrid.y).valid;
					bool isValidY = aStarGrid.cellGet(cellCurrent->cellPositionGrid.x, cellPosOffsetY).valid;

					// skip neighbor if you must pass through a walled corner to reach it, I.E. it is inaccessible
					if (!isValidX && isValidY) {
						continue;
					}
					// if x axis is valid, allow pathfinder to select neighbor on the y axis
					else if (!isValidX) {
						cellPosOffsetX -= x;
					}
					// if y axis is valid, allow pathfinder to select neighbor on the y axis
					else if (!isValidY) {
						cellPosOffsetY -= y;
					}
				}

				AStarCell& cellNeighbor = aStarGrid.cellGet(cellPosOffsetX, cellPosOffsetY);

				auto cellPosFind = [cellNeighbor](AStarCellPosition cellPos) { return cellPos == cellNeighbor.cellPositionGrid; };
				auto cellFind = [cellNeighbor](AStarCell* cell) { return *cell == cellNeighbor; };

				// skip current neighbor if they aren't valid or have already been explored
				if (!cellNeighbor.valid || std::find_if(cellsClosedVector.begin(), cellsClosedVector.end(), cellPosFind) != cellsClosedVector.end()) continue;

				// potential new costG for neighbor if path is shorter than existing neighborCur costG
				AStarCostValue neighborNewCostG = cellCurrent->costG + cellDistanceGet(cellCurrent->cellPositionGrid, cellNeighbor.cellPositionGrid);

				// whether the neighborCur is in the cellsOpenVector
				bool neighborIsInOpen = cellsOpenHeap.has(cellNeighbor);

				if (neighborNewCostG < cellNeighbor.costG || !neighborIsInOpen) {

					cellNeighbor.costG = neighborNewCostG;
					cellNeighbor.costH = cellDistanceGet(cellNeighbor.cellPositionGrid, cellEnd);
					cellNeighbor.costF = cellNeighbor.costG + cellNeighbor.costH;
					cellNeighbor.cellParent = cellCurrent;

					if (!neighborIsInOpen) {
						cellsOpenHeap.push(&aStarGrid.cellGet(cellNeighbor.cellPositionGrid));
					}
				}
			}
		}
	}

	// get finished path
	AStarPath path = pathSimplify(pathRetrace(aStarGrid.cellGet(cellStart), aStarGrid.cellGet(cellEnd)));

	return path;

}
AStarPath AStarPathfinder::pathGet(sf::Vector2f pointStart, sf::Vector2f pointEnd, AStarGrid& aStarGrid) {

	AStarCellPosition cellStart = aStarGrid.coordinatesWorldToCell(pointStart);
	AStarCellPosition cellEnd = aStarGrid.coordinatesWorldToCell(pointEnd);

	if (!aStarGrid.cellGet(cellStart).valid || !aStarGrid.cellGet(cellEnd).valid) {
		return AStarPath{ pointStart };
	}

	CellHeap cellsOpenHeap; // cells to be evaluated
	std::vector<AStarCellPosition> cellsClosedVector; // cells that have already been evaluated

	cellsOpenHeap.push(&aStarGrid.cellGet(cellStart));

	AStarCell* cellCurrent = cellsOpenHeap.top();

	while ((cellCurrent->cellPositionGrid != cellEnd) && (cellsOpenHeap.size() > 0)) {

		cellCurrent = cellsOpenHeap.top();
		cellsOpenHeap.pop();
		cellsClosedVector.push_back(cellCurrent->cellPositionGrid);

		for (int16_t x = -1; x <= 1; x++) {
			for (int16_t y = -1; y <= 1; y++) {

				if (x == 0 && y == 0) continue;

				int16_t cellPosOffsetX = int16_t(cellCurrent->cellPositionGrid.x + x);
				int16_t cellPosOffsetY = int16_t(cellCurrent->cellPositionGrid.y + y);

				if (!aStarGrid.cellPosIsInGrid(cellPosOffsetX, cellPosOffsetY)) continue;

				// if cell is a diagonal, make sure the pathfinder doesn't cut a corner
				if (x != 0 && y != 0) {
					bool isValidX = aStarGrid.cellGet(cellPosOffsetX, cellCurrent->cellPositionGrid.y).valid;
					bool isValidY = aStarGrid.cellGet(cellCurrent->cellPositionGrid.x, cellPosOffsetY).valid;

					// skip neighbor if you must pass through a walled corner to reach it, I.E. it is inaccessible
					if (!isValidX && isValidY) {
						continue;
					}
					// if x axis is valid, allow pathfinder to select neighbor on the y axis
					else if (!isValidX) {
						cellPosOffsetX -= x;
					}
					// if y axis is valid, allow pathfinder to select neighbor on the y axis
					else if (!isValidY) {
						cellPosOffsetY -= y;
					}
				}

				AStarCell& cellNeighbor = aStarGrid.cellGet(cellPosOffsetX, cellPosOffsetY);

				auto cellPosFind = [cellNeighbor](AStarCellPosition cellPos) { return cellPos == cellNeighbor.cellPositionGrid; };
				auto cellFind = [cellNeighbor](AStarCell* cell) { return *cell == cellNeighbor; };

				// skip current neighbor if they aren't valid or have already been explored
				if (!cellNeighbor.valid || std::find_if(cellsClosedVector.begin(), cellsClosedVector.end(), cellPosFind) != cellsClosedVector.end()) continue;

				// potential new costG for neighbor if path is shorter than existing neighborCur costG
				AStarCostValue neighborNewCostG = cellCurrent->costG + cellDistanceGet(cellCurrent->cellPositionGrid, cellNeighbor.cellPositionGrid);

				// whether the neighborCur is in the cellsOpenVector
				bool neighborIsInOpen = cellsOpenHeap.has(cellNeighbor);

				if (neighborNewCostG < cellNeighbor.costG || !neighborIsInOpen) {

					cellNeighbor.costG = neighborNewCostG;
					cellNeighbor.costH = cellDistanceGet(cellNeighbor.cellPositionGrid, cellEnd);
					cellNeighbor.costF = cellNeighbor.costG + cellNeighbor.costH;
					cellNeighbor.cellParent = cellCurrent;

					if (!neighborIsInOpen) {
						cellsOpenHeap.push(&aStarGrid.cellGet(cellNeighbor.cellPositionGrid));
					}
				}
			}
		}
	}

	// get finished path
	AStarPath path = pathSimplify(pathRetrace(aStarGrid.cellGet(cellStart), aStarGrid.cellGet(cellEnd)));

	return path;
}

AStarPath AStarPathfinder::pathGet(sf::Vector2f pointEnd, Entity& entity) {
	if (!entity.entityComponentHas<EntityComponents::ComponentPosition>()) {
		ConsoleHandler::consolePrintErr("AStarPathfinder \"pathGet\" function failed: Exception: entity does not have ComponentPosition");
		return AStarPath();
	}

	auto* entityComponentPosition = entity.entityComponentGet<EntityComponents::ComponentPosition>();

	sf::Vector2f pointStart = entityComponentPosition->position;

	AStarGrid& aStarGrid = GameLevelGrid::levelGet(entity.levelId)->aStarGrid;

	return pathGet(pointStart, pointEnd, aStarGrid);
}
AStarPath AStarPathfinder::pathGet(sf::Vector2f pointEnd, EntityId entityId) {
	return pathGet(pointEnd, EntityManager::entityGet(entityId));
}


