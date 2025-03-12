#ifndef __A_STAR_PATH_FINDER_H__
#define __A_STAR_PATH_FINDER_H__

#include "../../../ACECS/GameLevel.hpp"
#include "../Grid/AStarGrid.hpp"
#include <ECS.hpp>
#include <functional>

class AStarPathfinder {

	static AStarPath pathRetrace(AStarCell& cellStart, AStarCell& cellEnd);
	static AStarPath pathSimplify(AStarPath path);
	static void cellsResetData(std::vector<AStarCell*> cells);
	static AStarCostValue cellDistanceGet(AStarCellPosition cellPosFrom, AStarCellPosition cellPosTo);

public:
	static AStarPath pathGet(sf::Vector2f pointStart, sf::Vector2f pointEnd, AStarGrid& aStarGrid);
	static AStarPath pathGet(sf::Vector2f pointEnd, Entity& entity);
	static AStarPath pathGet(sf::Vector2f pointEnd, EntityId entityId);


	typedef std::function<uint16_t(AStarCell&)> SpecializationFunction;
	static AStarPath pathGet(sf::Vector2f pointStart, sf::Vector2f pointEnd, AStarGrid& aStarGrid, SpecializationFunction specializationFunction);
	static AStarPath pathGet(sf::Vector2f pointEnd, Entity& entity, SpecializationFunction specializationFunction);
	static AStarPath pathGet(sf::Vector2f pointEnd, EntityId entityId, SpecializationFunction specializationFunction);
};


#endif