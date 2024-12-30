#ifndef __A_STAR_GRID_H__
#define __A_STAR_GRID_H__

#include "../../../../Common/Grid.hpp"
#include "AStarCell.hpp"
#include <SFML/System/Vector2.hpp>
#include <vector>

typedef std::vector<sf::Vector2f> AStarPath;

class AStarGrid : public Grid<AStarCell> {
public:

	friend std::ofstream& operator<<(std::ofstream& str, AStarGrid& item);
	friend std::ifstream& operator>>(std::ifstream& str, AStarGrid& item);

	AStarGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);
};



#endif
