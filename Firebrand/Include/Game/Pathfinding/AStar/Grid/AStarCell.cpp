#include "AStarCell.hpp"
#include "AStarGrid.hpp"
#include <World/LevelGrid.hpp>
#include "../../../ACECS/GameLevel.hpp"
#include <iostream>

AStarCell::AStarCell() :
	cellPositionGrid(sf::Vector2u(0, 0)),
	cellPositionWorld(sf::Vector2f(0, 0))
{
	resetPathingData();
}

AStarCell::AStarCell(AStarCellPosition _cellPositionGrid, sf::Vector2f aStarGridCellSize) :
	cellPositionGrid(_cellPositionGrid)
{
	cellPositionWorld = sf::Vector2f(cellPositionGrid.x * aStarGridCellSize.x, cellPositionGrid.y * aStarGridCellSize.y) + (aStarGridCellSize / 2.f);

	resetPathingData();
}

bool AStarCell::operator< (const AStarCell& other) const {
	return (costF < other.costF);
}
bool AStarCell::operator<= (const AStarCell& other) const {
	return (costF <= other.costF);
}
bool AStarCell::operator> (const AStarCell& other) const {
	return (costF > other.costF);
}
bool AStarCell::operator>= (const AStarCell& other) const {
	return (costF >= other.costF);
}
bool AStarCell::operator== (const AStarCell& other) const {
	return cellPositionGrid == other.cellPositionGrid;
}
bool AStarCell::operator!= (const AStarCell& other) const {
	return cellPositionGrid != other.cellPositionGrid;
}
void AStarCell::operator= (const AStarCell& other) {
	costG = other.costG;
	costH = other.costH;
	costF = other.costF;

	valid = other.valid;
	cellParent = other.cellParent;

	cellPositionGrid.x = other.cellPositionGrid.x;
	cellPositionGrid.y = other.cellPositionGrid.y;
	cellPositionWorld.x = other.cellPositionWorld.x;
	cellPositionWorld.y = other.cellPositionWorld.y;
}

void AStarCell::resetCosts() {
	costG = 0;
	costH = 0;
	costF = 0;
}
void AStarCell::resetPathingData() {
	resetCosts();

	cellParent = nullptr;
}
