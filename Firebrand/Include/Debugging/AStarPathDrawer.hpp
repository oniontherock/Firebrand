#ifndef __A_STAR_PATH_DRAWER_H__
#define __A_STAR_PATH_DRAWER_H__

#include "../Game/Pathfinding/AStar/Grid/AStarGrid.hpp"
#include "../../ACECS/Panels.hpp"

class AStarPathDrawer {
public:

	static void pathsTextureReset();
	static void pathDraw(AStarPath path);

	static void cellsInvalidDraw(AStarGrid& aStarGrid);


	static sf::RenderTexture pathsTexture;


};


#endif

