#ifndef __PATH_VERIFIER_H__
#define __PATH_VERIFIER_H__

#include "../Wall Generation/WallGenerator.hpp"

class PathVerifier {



public:
	static bool pointsConnect(const WallGrid2D& wallGrid, sf::Vector2u structureSize, sf::Vector2u pointA, sf::Vector2u pointB);

};



#endif