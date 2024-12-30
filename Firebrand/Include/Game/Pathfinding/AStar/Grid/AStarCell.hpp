#ifndef __A_STAR_CELL_H__
#define __A_STAR_CELL_H__

#include <SFML/System/Vector2.hpp>
#include <vector>

// position of a cell in the AStarGrid
typedef sf::Vector2u AStarCellPosition;
// type used for costs
typedef uint16_t AStarCostValue;


struct AStarCell {

	AStarCell();
	AStarCell(AStarCellPosition _cellPositionGrid, sf::Vector2f aStarGridCellSize);

	AStarCell* cellParent;
	// g cost of the AStarCell
	AStarCostValue costG;
	// h cost of the AStarCell
	AStarCostValue costH;
	// f cost of the AStarCell
	AStarCostValue costF;


	// reset G, H, and F costs,
	void resetCosts();
	// resets all data that is modified during path generation,
	// this includes:
	// 
	// costG,
	// costH,
	// costF,
	void resetPathingData();

	bool operator< (const AStarCell& other) const;
	bool operator<= (const AStarCell& other) const;
	bool operator> (const AStarCell& other) const;
	bool operator>= (const AStarCell& other) const;
	bool operator== (const AStarCell& other) const;
	bool operator!= (const AStarCell& other) const;
	void operator= (const AStarCell& other);

	// whether this cell is valid for pathfinding, if not, the cell is treated like a wall
	bool valid = true;

	// position of the cell in the world
	sf::Vector2f cellPositionWorld;
	// position of the cell in the AStarGrid
	AStarCellPosition cellPositionGrid;

};


#endif