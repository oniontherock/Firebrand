#ifndef __PATH_AXIS_GENERATOR_H__
#define __PATH_AXIS_GENERATOR_H__

#include "../ACECS/GameLevel.hpp"
#include "Grid/PathAxisGrid.hpp"

namespace PathAxisGenerator {

	sf::Vector2f pointGetAxisToPathPointClosest(std::vector<sf::Vector2f> pathPoints, sf::Vector2f point);

	PathAxisGrid pathAxisGridGenerate(std::vector<sf::Vector2f> pathPoints, GameLevel* level);
}

#endif
